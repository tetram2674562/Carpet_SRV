//
// Created by tetram26 on 30/07/25.
//

#include "command/CommandHandler.h"

#include <string>

#include "server/VanillaMinecraftServer.h"
#include "utils/ConsoleUtils.h"

using namespace std;
using namespace utils;

namespace command {
// Command listener thread
CommandHandler CommandHandler::instance;
void CommandHandler::CommandListener(CommandHandler *instance) {
  std::string command;
  CommandHandler *self = instance;
  while (self->isRunning()) {
    ConsoleUtils::getInstance().getLine(command);
    std::vector<entity::Player *> playersSnapshot;
    {
      lock_guard lock(
          server::VanillaMinecraftServer::getServer().getPlayersMutex());
      playersSnapshot =
          server::VanillaMinecraftServer::getServer().getPlayers();
    }

    int nbPlayer = static_cast<int>(playersSnapshot.size());

    if (command == "stop") {
      ConsoleUtils::getInstance().printMessage("[Server] Stopping...");
      self->shutdown();
      server::VanillaMinecraftServer::getServer().shutdown();
    } else if (command == "kickall") {
      ConsoleUtils::getInstance().printMessage("[Server] Kick from console!");
      if (nbPlayer == 0) {
        ConsoleUtils::getInstance().printMessage(
            "[Server] There is no online players to kick...");
      } else {
        for (int i = 0; i < nbPlayer; ++i) {
          entity::Player *player = playersSnapshot[i];
          if (player) {
            server::VanillaMinecraftServer::getServer().requestKickPlayer(
                player, ConsoleUtils::createUTF16String(
                            "Kicked from the server by the console."));
          }
        }
        ConsoleUtils::getInstance().printMessage(
            "Kicked " + std::to_string(nbPlayer) + " players from the server.");
      }
    } else if (command == "list") {
      if (nbPlayer == 0) {
        ConsoleUtils::getInstance().printMessage(
            "[Server] There is no online players. ");
      } else {
        string playersList;
        for (size_t i = 0; i < playersSnapshot.size(); ++i) {
          entity::Player *player = playersSnapshot[i];
          if (player) {
            playersList += player->getName();
          }
          if (i != playersSnapshot.size() - 1) {
            playersList += ", ";
          }
        }
        ConsoleUtils::getInstance().printMessage(
            "There are " + to_string(nbPlayer) +
            " players online: " + playersList);
      }

    } else {
      ConsoleUtils::getInstance().printMessage("[Server] Unknown command: " +
                                               command);
    }

    std::this_thread::sleep_for(chrono::milliseconds(1000));
  }
}

void CommandHandler::initCommandHandler() {
  instance.command_listener_thread.detach();
}

CommandHandler::CommandHandler()
    : running(true), command_listener_thread(CommandListener, this) {}

CommandHandler &CommandHandler::getInstance() { return instance; }

void CommandHandler::shutdown() { this->running = false; }

bool CommandHandler::isRunning() const { return this->running; }
} // namespace command