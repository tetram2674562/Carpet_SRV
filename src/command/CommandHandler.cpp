//
// Created by tetram26 on 30/07/25.
//

#include "command/CommandHandler.h"

#include <iostream>
#include <string>

#include "server/VanillaMinecraftServer.h"
#include "utils/ConsoleUtils.h"
#include "utils/LockGuard.h"


using namespace std;

// Set this flag to false to shut down the server

namespace command {
    // Command listener thread
    CommandHandler CommandHandler::instance;
    void *CommandHandler::CommandListener(void* instance) {
        std::string command;
        CommandHandler* self = (CommandHandler*) instance;
        while (self->isRunning()) {
            utils::ConsoleUtils::getInstance().getLine(command);
            std::vector<entity::Player*> playersSnapshot;
            {
                LockGuard lock(server::VanillaMinecraftServer::getServer().getPlayersMutex());
                playersSnapshot = server::VanillaMinecraftServer::getServer().getPlayers();
            }

            int nbPlayer = static_cast<int>(playersSnapshot.size());

            if (command == "stop") {
                utils::ConsoleUtils::getInstance().printMessage("[Server] Stopping...");
                self->shutdown();
                server::VanillaMinecraftServer::getServer().shutdown();
            } else if (command == "kickall") {
                utils::ConsoleUtils::getInstance().printMessage("[Server] Kick from console!");
                if (nbPlayer == 0) {
                    utils::ConsoleUtils::getInstance().printMessage("[Server] There is no online players to kick...");
                } else {
                        for (int i = 0; i < nbPlayer; ++i) {
                            entity::Player* player = playersSnapshot[i];
                            if (player) {
                                server::VanillaMinecraftServer::getServer().requestKickPlayer(
                                        player,
                                        utils::ConsoleUtils::createUTF16String("Kicked from the server by the console.")
                                );
                            }
                        }
                    utils::ConsoleUtils::getInstance().printMessage("Kicked " + utils::ConsoleUtils::toString(nbPlayer) + " players from the server.");
                }
            } else if (command == "list") {
                if (nbPlayer == 0) {
                    utils::ConsoleUtils::getInstance().printMessage("[Server] There is no online players. ");
                } else {
                    string playersList;
                    for (size_t i = 0; i < playersSnapshot.size(); ++i) {
                        entity::Player* player = playersSnapshot[i];
                        if (player) {
                            playersList += player->getName();
                        }
                        if (i != playersSnapshot.size() - 1) {
                            playersList += ", ";
                        }
                    }
                    utils::ConsoleUtils::getInstance().printMessage("There are " + utils::ConsoleUtils::toString(nbPlayer) + " players online: " + playersList);
                }

            } else {
                utils::ConsoleUtils::getInstance().printMessage("[Server] Unknown command: " + command);
            }

            self->command_listener_thread.sleep(1000);
        }
        return 0;
    }

    void CommandHandler::initCommandHandler() {
        instance = CommandHandler();
        instance.command_listener_thread.createThread(CommandListener,&instance);
    }

    CommandHandler::CommandHandler(): running(true) {
    }

    CommandHandler& CommandHandler::getInstance(){
        return instance;
    }

    void CommandHandler::shutdown() {
        this->running = false;
    }

    bool CommandHandler::isRunning() const {
        return this->running;
    }
}