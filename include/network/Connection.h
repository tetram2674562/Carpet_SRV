//
// Created by tetram26 on 31/07/25.
//
#ifndef CONNECTION_H
#define CONNECTION_H


#include <ctime>

#include "utils/Mutex.h"
#include <vector>
#include <packet/Packet.h>

#include "crypto/AESCipher.h"
#include "packet/handshake/ServerPingPacket.h"
#include "utils/Queue.h"
#include "utils/UTF16String.h"

namespace entity {
    class Player;
}

namespace network {
    class Connection {
        public:
            Connection(int, entity::Player&);

            ~Connection();

            void addPacketToQueue(packet::Packet *);

            void sendAndFlushQueue();

            bool handleConnection(entity::Player &);
            void handlePackets();
            bool isAlive();
            void disconnect(const utils::UTF16String &);

            void handlePacket(packet::Buffer &buffer);

            void handlePacket(packet::ServerPingPacket &packet);

        private:
            // NO COPY
            Connection(const Connection&);           
            Connection& operator=(const Connection&);

            int client_FD;
            packet::Buffer dataBuffer;

            utils::Queue<packet::Packet*> queue;
            utils::Mutex queueMutex;

            utils::Mutex stateMutex;
            std::time_t lastActivity;
            crypto::AESCipher* cipher;
            entity::Player* player;

            bool performLoginSequence();
            bool recvPacket(std::vector<unsigned char> &);
    };
}
#endif //CONNECTION_H