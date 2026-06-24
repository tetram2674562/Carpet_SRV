//
// Created by tetram26 on 31/07/25.
//
#include <map>

#include "entity/player/Player.h"
#include "network/Connection.h"
#include "packet/handshake/ClientProtocolPacket.h"
#include "packet/handshake/KickPacket.h"
#include "packet/handshake/ServerPingPacket.h"
#include "packet/login/ClientStatusPacket.h"
#include "packet/login/LoginPacket.h"
#include "packet/login/ServerAuthDataPacket.h"
#include "packet/login/SharedKeyPacket.h"
#include "packet/play/KeepAlivePacket.h"
#include "packet/play/player/ClientInfoPacket.h"
#include "packet/play/player/PlayerPositionPacket.h"
#include "packet/play/player/SpawnPointPacket.h"
#include "utils/ConsoleUtils.h"

#include "utils/UTF16String.h"

#include <iostream>

using namespace std;
using namespace utils;

namespace network {

const std::map<unsigned char, Connection::PacketHandler>
    Connection::PACKETS_HANDLERS{
        {0xFE, &Connection::handleServerPingPacket},
        {0x02, &Connection::handleHandshake},
        {0xFC, &Connection::handleSharedKeyPacket},
        {0xCC, &Connection::handleClientInfo},
        {0x00, &Connection::handleKeepAlive},
        {0x0B, &Connection::handlePositionPacket},
        {0xCD,&Connection::handleClientStatus},
    };

Connection::Connection(asio::io_context &io_context)
    : socket_(io_context), running(true), mustDisconnect(false),
      cipher(nullptr), isWriting(false), lastActivity(time(nullptr)),
      player(nullptr), counter(0), status(HANDSHAKE) {}

Connection::pointer Connection::create(asio::io_context &io_context) {
  return pointer(new Connection(io_context));
}
Connection::~Connection() {
  lock_guard ciphLock(cipherMutex);
  if (this->cipher != nullptr) {
    delete this->cipher;
    this->cipher = nullptr;
  }
}
void Connection::addPacketToQueue(packet::Packet *packet) {

  {
    lock_guard qlg(queueMutex);
    this->queue.push_back(packet);
  }
  if (!isWriting)
    start_write();
}
void Connection::handleConnection(entity::Player &_player) {
  this->player = &_player;
  performLoginSequence();
}

UTF16String serializeServerInfo(const std::string &version,
                                const std::string &motd,
                                const int currentPlayers,
                                const int maxPlayers) {
  std::vector<UTF16String> data;
  data.emplace_back("1");
  data.emplace_back("51");
  data.emplace_back(version);
  data.emplace_back(motd);
  data.emplace_back(std::to_string(currentPlayers));
  data.emplace_back(to_string(maxPlayers));

  UTF16String joined;
  joined.append(0x00A7);

  for (const UTF16String &i : data) {
    joined += i;
    joined.append(0x0000);
  }

  return joined;
}

void Connection::performLoginSequence() { start_read(); }

bool Connection::isAlive() const { return this->running; }

void Connection::disconnect(const UTF16String &reason) {
  auto kickPacket = new packet::KickPacket(reason);
  addPacketToQueue(kickPacket);
  running = false;
  if (this->player) {
    ConsoleUtils::getInstance().printMessage("Disconnected: " +
                                             this->player->getName());
  } else {
    ConsoleUtils::getInstance().printMessage("Disconnected: (unknown)");
  }
  mustDisconnect = true;
}

void Connection::disconnect() {
  running = false;
  ConsoleUtils::getInstance().printMessage("Disconnected: (unknown)");
  mustDisconnect = true;
}

void Connection::start_read() {
  if (readBuffer.read_pos() >= readBuffer.size()) {
    readBuffer.clearBuffer();
  }

  socket_.async_read_some(readBuffer.mutableBuffer(),
                          std::bind(&Connection::handle_read,
                                    this->shared_from_this(), placeholders::_1,
                                    placeholders::_2));
}

void Connection::handle_read(const asio::error_code &error, std::size_t size) {
  if (!error && size > 0) {
    if (cipher != nullptr) {
      lock_guard ciphLock(cipherMutex);
      unsigned char* newBytesPtr = readBuffer.getDataBuffer().data() + readBuffer.read_pos();
      cipher->decrypt(newBytesPtr, newBytesPtr, size);
    }

    process_packets(readBuffer.read_pos() + size);
    start_read();
  }
}
void Connection::process_packets(const size_t size) {
  try {
    while (readBuffer.read_pos() < size) {
      unsigned char packetId = readBuffer.readByte();

      auto it = PACKETS_HANDLERS.find(packetId);
      if (it != PACKETS_HANDLERS.end()) {
        (this->*it->second)();
      } else {
        std::stringstream ss;
        ss << "Unknown packet id: 0x" << std::hex << (int)packetId;
        ConsoleUtils::getInstance().printerr(ss.str());
        break;
      }
    }
  } catch (const std::runtime_error &e) {
    readBuffer.clearBuffer();
  }
}

void Connection::start_write() {
  if (isWriting)
    return;
  isWriting = true;
  using packet::Packet;
  {
    lock_guard qlg(queueMutex);
    for (auto it = queue.begin(); it != queue.end();) {
      Packet *packet = *it;
      packet->writeData(writeBuffer);
      delete packet;
      it = queue.erase(it);
    }
  }
  {
    if (cipher != nullptr) {
      lock_guard ciphLock(cipherMutex);
      writeBuffer.encrypt(*cipher);
    }
  }
  asio::async_write(socket_, writeBuffer.constBuffer(),
                    std::bind(&Connection::handle_write, shared_from_this(),
                              placeholders::_1, placeholders::_2));
}
void Connection::handle_write(const asio::error_code &error, std::size_t size) {

  isWriting = false;
  if (!error) {
    if (size != 0 && status == PING) {
      disconnect();
    } else {
      writeBuffer.clearBuffer();
    }
  }

  if (mustDisconnect) {
    socket_.close();
    running = false;
  }
}
asio::ip::tcp::socket &Connection::socket() { return socket_; }
void Connection::sendKeepAlive() {
  if (status == PLAY) {
    addPacketToQueue(new packet::KeepAlivePacket(++counter));
  }
  if (time(nullptr) - lastActivity > 20) {
    status == PLAY ? disconnect(UTF16String("Timed out")) : disconnect();
  }
}

///
///  _____________________HANDLERS__________________________________
///

void Connection::handleServerPingPacket() {
  if (status == HANDSHAKE) {
    packet::ServerPingPacket serverPing;
    serverPing.readData(readBuffer);
    auto pingResponsePacket = new packet::KickPacket(
        serializeServerInfo("1.4.7", "A server running Carpet SRV", 0, 20));
    addPacketToQueue(pingResponsePacket);
    status = PING;
  }
}

void Connection::handleHandshake() {
  if (status == HANDSHAKE) {
    packet::ClientProtocolPacket clientProtocolPacket;
    clientProtocolPacket.readData(readBuffer);
    if (clientProtocolPacket.getProtocolVersion() != 51) {
      disconnect(UTF16String("pinged."));
      ConsoleUtils::getInstance().printMessage("Invalid version");
    }
    if (this->player) {
      this->player->setUsername(clientProtocolPacket.getUsername());
      ConsoleUtils::getInstance().printMessage(
          "[Server] Got connection from " +
          clientProtocolPacket.getServerHost() + " with username \"" +
          this->player->getName() + "\"");
    }
    auto serverAuthDataPacket = new packet::ServerAuthDataPacket;
    verifyToken = serverAuthDataPacket->getVerifyToken();

    addPacketToQueue(serverAuthDataPacket);
    status = LOGIN;
  }
}
void Connection::handleSharedKeyPacket() {
  if (status == LOGIN) {
    packet::SharedKeyPacket* sharedKeyPacket = new packet::SharedKeyPacket;
    sharedKeyPacket->readData(readBuffer);
    if (verifyToken != sharedKeyPacket->getVerifyToken()) {
      disconnect();
      ConsoleUtils::getInstance().printMessage(
          "Expected the exact same verify token");
    }
    vector<unsigned char> val = sharedKeyPacket->getSharedSecret();


    addPacketToQueue(sharedKeyPacket);
    {
      lock_guard ciphLock(cipherMutex);
      this->cipher = new crypto::AESCipher(val);
    }
  }
}

void Connection::handleClientInfo() {
  packet::ClientInfoPacket infoPacket;
  infoPacket.readData(readBuffer);
  if (player) {
    player->setLanguage(infoPacket.getLanguage());
    player->setShowCape(infoPacket.getShowCape());
    player->setRenderDistance(infoPacket.getRenderDistance());
  }
}
void Connection::handleKeepAlive() {
    lastActivity = time(nullptr);
    ConsoleUtils::getInstance().printMessage("keep alive");
}
void Connection::handlePositionPacket() {
  packet::PlayerPositionPacket positionPacket;
  positionPacket.readData(readBuffer);
  if (player) {
    player->setPosition(positionPacket.getX(), positionPacket.getY(),
                        positionPacket.getZ());
  }
}

void Connection::handleClientStatus() {
  packet::ClientStatusPacket statusPacket;
  statusPacket.readData(readBuffer);

  if (statusPacket.getStatus() == 0) {
    status = PLAY;
    auto *loginPacket = new packet::LoginPacket;
    addPacketToQueue(loginPacket);
    addPacketToQueue(new packet::SpawnPointPacket(0,0,0));
  }
}

} // namespace network
