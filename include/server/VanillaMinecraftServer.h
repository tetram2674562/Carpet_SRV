//
// Created by tetram26 on 30/07/25.
//
#ifndef VANILLAMINECRAFTSERVER_H
#define VANILLAMINECRAFTSERVER_H
#include "crypto/KeyPair.h"
#include "entity/player/Player.h"
#include "utils/ConsoleUtils.h"
#include <tbb/tbb.h>
#include <asio.hpp>
#define PORT 25565
#define TICK_RATE 20

namespace server {
const int TICK_INTERVAL_MS = 1000 / TICK_RATE;
class VanillaMinecraftServer
{

public:
  static void startServer();
  static VanillaMinecraftServer& getServer();

  void tick();

  void shutdown();
  bool isRunning();

  void requestKickPlayer(const entity::Player*, const utils::UTF16String&);

  std::vector<entity::Player*>& getPlayers();

  /// Function to get the keypair
  crypto::KeyPair& getKeyPair();

  std::mutex& getPlayersMutex();

  void start_accept();
  void handle_accept(const network::Connection::pointer &new_connection,
                     const std::error_code &error);

private:
  VanillaMinecraftServer(asio::io_context&);

  crypto::KeyPair keypair;
  static VanillaMinecraftServer* server;
  std::atomic<bool> running;

  // Asio networking stuff
  asio::io_context &io_context_;
  asio::ip::tcp::acceptor acceptor_;

  std::vector<entity::Player*> players;
  std::vector<int> usedIDs;
  std::mutex playersMutex;
  int maxPlayers;
  std::mutex serverStateMutex;
};
}
#endif // VANILLAMINECRAFTSERVER_H