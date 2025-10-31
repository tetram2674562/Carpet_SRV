//
// Created by tetram26 on 30/07/25.
//
#ifndef VANILLAMINECRAFTSERVER_H
#define VANILLAMINECRAFTSERVER_H
#include "crypto/KeyPair.h"
#include "entity/player/Player.h"
#include "utils/ConsoleUtils.h"
#include "utils/ThreadUtils.h"
#define PORT 25565
#define TICK_RATE 20

namespace server {
    const int TICK_INTERVAL_MS = 1000 / TICK_RATE;
    class VanillaMinecraftServer {
        // TODO Sort this.
        public:
            static void startServer();
            static VanillaMinecraftServer& getServer();

            static void handleClient(int);

            void tick();

            static void *ServerThread(void *);

            void shutdown();
            bool isRunning();

            void requestKickPlayer(const entity::Player*, const std::basic_string<unsigned short> &);

            std::vector<entity::Player *> &getPlayers();

            crypto::KeyPair& getKeyPair();
            int getServerSocket() const;

            void setServerSocket(int);
            Mutex& getPlayersMutex();
        private:
            VanillaMinecraftServer();
            Thread serverThread;
            crypto::KeyPair keypair;
            static VanillaMinecraftServer server;
            bool running;
            int server_socket;
            std::vector<entity::Player*> players;
            std::vector<int> usedIDs;
            Mutex playersMutex;
            int maxPlayers;
            Mutex serverStateMutex;
    };
}
#endif //VANILLAMINECRAFTSERVER_H