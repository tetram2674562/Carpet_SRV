//
// Created by tetram26 on 30/07/25.
//

#include "server/VanillaMinecraftServer.h"

#include <openssl/rand.h>

#include "command/CommandHandler.h"
#include "network/Connection.h"
#include "utils/ConsoleUtils.h"
using namespace std;
using namespace utils;
using namespace asio;

namespace server {

VanillaMinecraftServer* VanillaMinecraftServer::server;

VanillaMinecraftServer::VanillaMinecraftServer(io_context& io_context)
  : keypair(1024)
  , running(true)
  , io_context_(io_context)
  , acceptor_(io_context_, ip::tcp::endpoint(ip::tcp::v4(), 25565))
  , maxPlayers(20)
{
  start_accept();
}

void
VanillaMinecraftServer::startServer()
{
  using network::Connection;
  io_context io_context;
  auto work_guard = make_work_guard(io_context);
  server = new VanillaMinecraftServer(io_context);
  command::CommandHandler::initCommandHandler();

  ConsoleUtils::getInstance().printMessage("Server listening on port " +
                                           std::to_string(PORT) + "...");

  std::thread networkThread([&io_context]{
    io_context.run();
  });
  while (server->isRunning()) {
    server->tick();

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  // cleanup
  io_context.stop();
  networkThread.join();

  OPENSSL_cleanup();
}

void
VanillaMinecraftServer::shutdown()
{
  lock_guard lock_players(this->playersMutex);
  lock_guard lock_state(this->serverStateMutex);

  // Kick and delete all players on shutdown
  for (size_t i = 0; i < this->players.size(); i++) {
    if (this->players[i] != nullptr) {
      this->players[i]->kickPlayer(
        ConsoleUtils::createUTF16String("Server is restarting..."));
      delete this->players[i];
    }
  }
  this->players.clear();
  this->running = false;
  io_context_.stop();
}

void
VanillaMinecraftServer::requestKickPlayer(const entity::Player* player,
                                          const UTF16String& reason)
{
  lock_guard lock(this->playersMutex);
  for (size_t i = 0; i < this->players.size(); ++i) {
    if (this->players[i] == player) {
      if (this->players[i] != nullptr) {
        this->players[i]->kickPlayer(reason);
        delete this->players[i];
      }
      this->players.erase(this->players.begin() + i);
      return;
    }
  }
}

/** Get the instance of the server
 *
 * @return The instance of the current running minecraft server
 */
VanillaMinecraftServer&
VanillaMinecraftServer::getServer()
{
  return *server;
}

/** Return the keypair to the keypair
 *
 * @return The keypair
 */
crypto::KeyPair&
VanillaMinecraftServer::getKeyPair()
{
  return this->keypair;
}

/** Check if the server is running and not shutting down
 *
 * @return true if the server should be running, else false
 */
bool
VanillaMinecraftServer::isRunning()
{
  return this->running;
}

/** Tick worlds and the players
 *
 */
void
VanillaMinecraftServer::tick()
{
  lock_guard lock_players(this->playersMutex);
  for (size_t i = 0; i < players.size();) {
    entity::Player* player = this->players[i];
    if (player == nullptr) {
      players.erase(players.begin() + i);
      continue;
    }
    if (!player->getConnection().isAlive()) {
      ConsoleUtils::getInstance().printMessage(
        "Client " + player->getName() + " disconnected.");
      player->kickPlayer(ConsoleUtils::createUTF16String("Ligma balls"));
      delete player;
      players.erase(players.begin() + i);
    } else {
      ++i;
    }
  }
}

std::mutex&
VanillaMinecraftServer::getPlayersMutex()
{
  return this->playersMutex;
}
void
VanillaMinecraftServer::start_accept()
{
  using network::Connection;

  Connection::pointer new_connection = Connection::create(io_context_);

  acceptor_.async_accept(new_connection->socket(),
                         std::bind(&VanillaMinecraftServer::handle_accept,
                                   this,
                                   new_connection,
                                   asio::placeholders::error));
}

void
VanillaMinecraftServer::handle_accept(
  const network::Connection::pointer& new_connection,
  const error_code& error)
{
  if (!error) {
    ConsoleUtils::getInstance().printMessage(
      "New connection encountered !");
    const auto player = new entity::Player(new_connection);
    {
      lock_guard lock(playersMutex);
      players.push_back(player);
    }
    player->handleConnection();
  }

  start_accept();
}

vector<entity::Player*>&
VanillaMinecraftServer::getPlayers()
{
  return this->players;
}

}
