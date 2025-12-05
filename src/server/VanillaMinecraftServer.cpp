//
// Created by tetram26 on 30/07/25.
//

#include "server/VanillaMinecraftServer.h"

#include <cerrno>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <openssl/rand.h>

#include "command/CommandHandler.h"
#include "network/Connection.h"
#include "utils/ConsoleUtils.h"
#include "utils/ThreadUtils.h"
#include "utils/TimeUtils.h"
#include "utils/LockGuard.h"
using namespace std;
using namespace utils;
namespace server {
    VanillaMinecraftServer VanillaMinecraftServer::server;
    VanillaMinecraftServer::VanillaMinecraftServer() : keypair(1024), running(true), server_socket(-1), maxPlayers(20) {}

    void VanillaMinecraftServer::startServer() {
        VanillaMinecraftServer& s = server;
        s.serverThread.createThread(ServerThread, &s);
        s.serverThread.joinThread();
    }


    void* VanillaMinecraftServer::ServerThread(void* server_ptr) {
        VanillaMinecraftServer* server = static_cast<VanillaMinecraftServer *>(server_ptr);
        command::CommandHandler::initCommandHandler();

        const int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        sockaddr_in serverAddr = {};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        const int flags = fcntl(serverSocket, F_GETFL, 0);
        fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);
        const int bindOutput = bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
        if (bindOutput != 0) {
            utils::ConsoleUtils::getInstance().printerr("Failed to bind to port " + utils::ConsoleUtils::toString(PORT));
            return 0;
        }
        listen(serverSocket, 5);

        utils::ConsoleUtils::getInstance().printMessage("Server listening on port " + utils::ConsoleUtils::toString(PORT) + "...");
        server->setServerSocket(serverSocket);
        while (server->isRunning()) {
            double start = TimeUtils::getTimeMillis();

            server->tick(); // process all clients
            sockaddr_in clientAddr = {};
            socklen_t clientSize = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientSize);
            if (clientSocket < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    server->serverThread.sleep(100);
                    continue;
                }
                perror("accept failed");
            }
            handleClient(clientSocket);
            double end = TimeUtils::getTimeMillis();
            double duration = end - start;
            double sleepTime = TICK_INTERVAL_MS - duration;
            if (sleepTime > 0) {
                server->serverThread.sleep(sleepTime);
            }
        }
        close(serverSocket);
        OPENSSL_cleanup();
        return 0;
    }

    void VanillaMinecraftServer::shutdown() {
        LockGuard lock_players(this->playersMutex);
        LockGuard lock_state(this->serverStateMutex);

        // Kick and delete all players on shutdown
        for (size_t i = 0; i < this->players.size(); i++) {
            if (this->players[i] != NULL) {
                this->players[i]->kickPlayer(utils::ConsoleUtils::createUTF16String("Server is restarting..."));
                delete this->players[i];
            }
        }
        this->players.clear();
        this->running = false;

    }

    void VanillaMinecraftServer::requestKickPlayer(const entity::Player* player, const utils::UTF16String & reason) {
        LockGuard lock(this->playersMutex);
        for (size_t i = 0; i < this->players.size(); ++i) {
            if (this->players[i] == player) {
                if (this->players[i] != NULL) {
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
    VanillaMinecraftServer &VanillaMinecraftServer::getServer() {
        return server;
    }

    /** Return the keypair to the keypair
     *
     * @return The keypair
     */
    crypto::KeyPair& VanillaMinecraftServer::getKeyPair() {
        return this->keypair;
    }

    /** Check if the server is running and not shutting down
     * 
     * @return true if the server should be running, else false
     */
    bool VanillaMinecraftServer::isRunning() {
        LockGuard lock(this->serverStateMutex);
        return this->running;
    }


    /** Handle a new client connection
     *
     * @param clientSocket the client socket
     */
    void VanillaMinecraftServer::handleClient(const int clientSocket) {
        LockGuard lock(server.playersMutex);
        entity::Player* player = new entity::Player(clientSocket);
        if (player->handleConnection()) {
            server.players.push_back(player);
        } else {
            // Connection failed during login / setup, free immediately
            delete player;
        }
    }
    /** Tick worlds and the players
     *
     */
    void VanillaMinecraftServer::tick() {
        LockGuard lock_players(this->playersMutex);
        for (size_t i = 0; i < players.size(); ) {
            entity::Player* player = this->players[i];
            if (player == NULL) {
                players.erase(players.begin() + i);
                continue;
            }

            player->getConnection().handlePackets();
            if (!player->getConnection().isAlive()) {
                utils::ConsoleUtils::getInstance().printMessage("Client " + player->getName() + " disconnected.");
                player->kickPlayer(utils::ConsoleUtils::createUTF16String("Ligma balls"));
                delete player;
                players.erase(players.begin() + i);
            } else {
                ++i;
            }
        }
    }

    /** Get the server socket (why du fuck do you need that for?)
     * 
     * @return the server socket
     */
    int VanillaMinecraftServer::getServerSocket() const {
        return this->server_socket;
    }

    void VanillaMinecraftServer::setServerSocket(const int serverSocket) {
        this->server_socket = serverSocket;
    }

    Mutex & VanillaMinecraftServer::getPlayersMutex() {
        return this->playersMutex;
    }

    vector<entity::Player *> &VanillaMinecraftServer::getPlayers(){
        return this->players;
    }

}

