//
// Created by tetram26 on 31/07/25.
//
#include <map>

#include "network/Connection.h"
#include "entity/player/Player.h"
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
    {0xFE,&Connection::handleServerPingPacket},
    {0x02,&Connection::handleHandshake},
    {0xFC,&Connection::handleSharedKeyPacket},
    {0xCC,&Connection::handleClientInfo},
    {0x00,&Connection::handleKeepAlive},
    {0x0B,&Connection::handlePositionPacket},
  };

Connection::Connection(asio::io_context& io_context)
  : socket_(io_context)
  , queue(20)
  , lastActivity(time(nullptr))
  , player(nullptr)
  , status(HANDSHAKE)
{
  this->cipher = nullptr;
}

Connection::pointer
Connection::create(asio::io_context& io_context)
{
  return pointer(new Connection(io_context));
}

void
Connection::addPacketToQueue(packet::Packet* packet)
{
  lock_guard qlg(this->queueMutex);
  this->queue.push_back(packet);
}
void
Connection::handleConnection(entity::Player& player)
{
  this->player = &player;
  performLoginSequence();
}

UTF16String
serializeServerInfo(const std::string& version,
                    const std::string& motd,
                    const int currentPlayers,
                    const int maxPlayers)
{
  std::vector<UTF16String> data;
  data.emplace_back("1");
  data.emplace_back("51");
  data.emplace_back(version);
  data.emplace_back(motd);
  data.emplace_back(std::to_string(currentPlayers));
  data.emplace_back(to_string(maxPlayers));

  UTF16String joined;
  joined.append(0x00A7);

  for (const UTF16String& i : data) {
    joined += i;
    joined.append(0x0000);
  }

  return joined;
}

void
Connection::performLoginSequence()
{
  start_read();
  start_write();
}

bool
Connection::isAlive() const
{
  return this->socket_.is_open();
}

void
Connection::disconnect(const UTF16String& reason)
{
  packet::KickPacket* kickPacket = new packet::KickPacket(reason);
  addPacketToQueue(kickPacket);
  socket_.close();

  if (this->player) {
    ConsoleUtils::getInstance().printMessage("Disconnected: " +
                                             this->player->getName());
  } else {
    ConsoleUtils::getInstance().printMessage("Disconnected: (unknown)");
  }
}

void
Connection::start_read()
{

  socket_.async_read_some(readBuffer.mutableBuffer(),
                          std::bind(&Connection::handle_read,
                                    this->shared_from_this(),
                                    asio::placeholders::error,
                                    asio::placeholders::bytes_transferred));
}

void
Connection::handle_read(const std::error_code& error, std::size_t)
{
  if (!error) {
    if (cipher != nullptr)
      readBuffer.decrypt(*cipher);

    bool read = true;
    while (read) {
      try {
        if (auto handler = PACKETS_HANDLERS.find(readBuffer.readByte());
            handler != PACKETS_HANDLERS.end()) {
          (this->*handler->second)();
        }
      } catch (std::runtime_error&) {
        read = false;
        readBuffer.clearBuffer();
      }
      this_thread::sleep_for(chrono::milliseconds(20));
    }

    if (time(nullptr) - lastActivity > 20) {
      disconnect(UTF16String("Timed out"));
    }
    lastActivity = time(nullptr);
    start_read();
  }
}
void
Connection::start_write()
{
  using packet::Packet;
  {
    lock_guard qlg(queueMutex);
    for (const auto it = queue.begin(); it != queue.end();) {
      Packet* packet = *it;
      packet->writeData(writeBuffer);
      delete packet;
      queue.erase(it);
    }
  }
  if (cipher != nullptr)
    writeBuffer.encrypt(*cipher);
  asio::async_write(socket_,
                    writeBuffer.constBuffer(),
                    std::bind(&Connection::handle_write,
                              shared_from_this(),
                              asio::placeholders::error,
                              asio::placeholders::bytes_transferred));
}
void
Connection::handle_write(const std::error_code& error, std::size_t)
{
  if (!error) {
    writeBuffer.clearBuffer();
    start_write();
  }
}
asio::ip::tcp::socket&
Connection::socket()
{
  return socket_;
}

///
///  _____________________HANDLERS__________________________________
///

void
Connection::handleServerPingPacket()
{
  if (status == HANDSHAKE) {
    packet::ServerPingPacket serverPing;
    serverPing.readData(readBuffer);
    packet::KickPacket pingResponsePacket(
      serializeServerInfo("1.4.7", "A server running Carpet SRV", 0, 20));
    addPacketToQueue(&pingResponsePacket);
  }
  socket_.close();
}

void
Connection::handleHandshake()
{
  if (status == HANDSHAKE) {
    packet::ClientProtocolPacket clientProtocolPacket;
    clientProtocolPacket.readData(readBuffer);
    if (clientProtocolPacket.getProtocolVersion() != 51) {
      socket_.close();
      ConsoleUtils::getInstance().printMessage("Invalid version");
    }
    if (this->player)
      this->player->setUsername(clientProtocolPacket.getUsername());
    ConsoleUtils::getInstance().printMessage(
      "[Server] Got connection from " + clientProtocolPacket.getServerHost() +
      " with username \\\"" + this->player->getName() + "\\\"");

    packet::ServerAuthDataPacket serverAuthDataPacket;
    addPacketToQueue(&serverAuthDataPacket);
    verifyToken = serverAuthDataPacket.getVerifyToken();
    status = LOGIN;
  } else {
    socket_.close();
  }
}
void
Connection::handleSharedKeyPacket()
{
  if (status == LOGIN) {
    packet::SharedKeyPacket sharedKeyPacket;
    sharedKeyPacket.readData(readBuffer);
    if (verifyToken != sharedKeyPacket.getVerifyToken()) {
      socket_.close();
      ConsoleUtils::getInstance().printMessage(
        "Excepted an exact same verify token");
    }

    this->cipher = new crypto::AESCipher(sharedKeyPacket.getSharedSecret());
    addPacketToQueue(&sharedKeyPacket);
    packet::LoginPacket loginPacket;
    addPacketToQueue(&loginPacket);
    status = PLAY;
  } else {
    socket_.close();
  }
}

void
Connection::handleClientInfo()
{
  packet::ClientInfoPacket infoPacket;
  infoPacket.readData(readBuffer);
  if (player) {
    player->setLanguage(infoPacket.getLanguage());
    player->setShowCape(infoPacket.getShowCape());
    player->setRenderDistance(infoPacket.getRenderDistance());
  }
}
void
Connection::handleKeepAlive()
{
  packet::KeepAlivePacket* keepAlivePacket =
    new packet::KeepAlivePacket();
  addPacketToQueue(keepAlivePacket);
}
void
Connection::handlePositionPacket()
{
  packet::PlayerPositionPacket positionPacket;
  positionPacket.readData(readBuffer);
  if (player) {
    player->setPosition(positionPacket.getX(),
                              positionPacket.getY(),
                              positionPacket.getZ());
  }
}

}
