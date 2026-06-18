//
// Created by tetram26 on 31/07/25.
//
#include <map>

#include "entity/player/Player.h"
#include "network/Connection.h"
#include "packet/handshake/ClientProtocolPacket.h"
#include "packet/handshake/KickPacket.h"
#include "packet/handshake/ServerPingPacket.h"
#include "packet/login/LoginPacket.h"
#include "packet/login/ServerAuthDataPacket.h"
#include "packet/login/SharedKeyPacket.h"
#include "packet/play/KeepAlivePacket.h"
#include "packet/play/player/ClientInfoPacket.h"
#include "packet/play/player/PlayerPositionPacket.h"
#include "utils/ConsoleUtils.h"

#include "utils/UTF16String.h"

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
    };

Connection::Connection(asio::io_context &io_context)
    : socket_(io_context), running(true), mustDisconnect(false),
      isWriting(false), lastActivity(time(nullptr)), player(nullptr),
      status(HANDSHAKE) {
  this->cipher = nullptr;
}

Connection::pointer Connection::create(asio::io_context &io_context) {
  return pointer(new Connection(io_context));
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
  if (!error) {
    if (cipher != nullptr)
      readBuffer.decrypt(*cipher);

    process_packets(size);

    if (time(nullptr) - lastActivity > 20) {
      disconnect(UTF16String("Timed out"));
    }
    lastActivity = time(nullptr);
    start_read();
  }
}

void Connection::process_packets(const size_t size) {
  try {
    while (readBuffer.read_pos() < size) {
      if (readBuffer.read_pos() >= readBuffer.size())
        break;

      unsigned char packetId = readBuffer.readByte();

      auto it = PACKETS_HANDLERS.find(packetId);
      if (it != PACKETS_HANDLERS.end()) {
        (this->*it->second)();
      } else {
        ConsoleUtils::getInstance().printerr("Unknown packet id: " +
                                             std::to_string(packetId));

        break;
      }
    }
  } catch (const std::runtime_error &) {
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
  if (cipher != nullptr)
    writeBuffer.encrypt(*cipher);
  asio::async_write(socket_, writeBuffer.constBuffer(),
                    std::bind(&Connection::handle_write, shared_from_this(),
                              placeholders::_1, placeholders::_2));
}
void Connection::handle_write(const asio::error_code &error, std::size_t size) {

  isWriting = false;
  if (!error) {
    if (size != 0 && status == PING) {
      disconnect(UTF16String("pinged"));
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
          clientProtocolPacket.getServerHost() + " with username \\\"" +
          this->player->getName() + "\\\"");
    }
    auto serverAuthDataPacket = new packet::ServerAuthDataPacket;
    addPacketToQueue(serverAuthDataPacket);
    verifyToken = serverAuthDataPacket->getVerifyToken();
    status = LOGIN;
  }
}
void Connection::handleSharedKeyPacket() {
  if (status == LOGIN) {
    auto *sharedKeyPacket = new packet::SharedKeyPacket;
    sharedKeyPacket->readData(readBuffer);
    if (verifyToken != sharedKeyPacket->getVerifyToken()) {
      ConsoleUtils::getInstance().printMessage(
          "Expected the exact same verify token");
    }
    this->cipher = new crypto::AESCipher(sharedKeyPacket->getSharedSecret());
    addPacketToQueue(sharedKeyPacket);
    auto *loginPacket = new packet::LoginPacket;
    addPacketToQueue(loginPacket);
    status = PLAY;
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
  auto keepAlivePacket = new packet::KeepAlivePacket();
  addPacketToQueue(keepAlivePacket);
}
void Connection::handlePositionPacket() {
  packet::PlayerPositionPacket positionPacket;
  positionPacket.readData(readBuffer);
  if (player) {
    player->setPosition(positionPacket.getX(), positionPacket.getY(),
                        positionPacket.getZ());
  }
}

} // namespace network
