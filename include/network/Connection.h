//
// Created by tetram26 on 31/07/25.
//
#ifndef CONNECTION_H
#define CONNECTION_H

#include <ctime>

#include "crypto/AESCipher.h"
#include "packet/play/KeepAlivePacket.h"
#include "utils/ConsoleUtils.h"
#include "utils/UTF16String.h"
#include <map>
#include <packet/Packet.h>
#include <vector>

#include <asio.hpp>

namespace entity {
class Player;
}

namespace network {
class Connection : public std::enable_shared_from_this<Connection> {
public:
  typedef std::shared_ptr<Connection> pointer;

  static pointer create(asio::io_context &io_context);
  ~Connection();

  // NO COPY
  Connection(const Connection &) = delete;
  Connection &operator=(const Connection &) = delete;

  /**
   * \brief Allow you to add packet to be send to the connection
   */
  void addPacketToQueue(packet::Packet *);

  void handleConnection(entity::Player &);

  bool isAlive() const;
  void disconnect(const utils::UTF16String &);
  void disconnect();

  // Read tcp
  void start_read();
  void handle_read(const asio::error_code &error, std::size_t);
  void process_packets(size_t size);

  // Write tcp
  void start_write();
  void handle_write(const asio::error_code &error, std::size_t);
  asio::ip::tcp::socket &socket();
  void sendKeepAlive();

  enum Status { HANDSHAKE, LOGIN, PLAY, PING };

private:
  Connection(asio::io_context &);

  // Socket
  asio::ip::tcp::socket socket_;
  std::atomic_bool running;
  std::atomic_bool mustDisconnect;
  // Buffers
  packet::Buffer writeBuffer;
  packet::Buffer readBuffer;
  // Cryptographic cipher AES128
  crypto::AESCipher *cipher;
  mutable std::mutex cipherMutex;

  // Packet queue
  std::vector<packet::Packet *> queue;
  std::mutex queueMutex;
  std::atomic<bool> isWriting;

  std::mutex stateMutex;
  std::time_t lastActivity;
  entity::Player *player;

  // Verify token for crypto
  std::vector<unsigned char> verifyToken;


  int counter;

  // Status of the connection
  Status status;

  // Handlers
  void handleServerPingPacket(); // 0xFE
  void handleHandshake();        // 0x02
  void handleSharedKeyPacket();  // 0xFC
  void handleClientInfo();       // 0xCC
  void handleKeepAlive();        // 0x00
  void handlePositionPacket();   // 0x0B
  void handleClientStatus();
  void handleDisconnectPacket();
  void handleTimePacket();

  typedef void (Connection::*PacketHandler)();

  static const std::map<unsigned char, PacketHandler> PACKETS_HANDLERS;
  void performLoginSequence();
};
} // namespace network
#endif // CONNECTION_H