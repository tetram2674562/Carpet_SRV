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
            void disconnect(const std::basic_string<unsigned short> &);
        private:
            // NO COPY
            Connection(const Connection&);           
            Connection& operator=(const Connection&);

            int client_FD;
            packet::Buffer dataBuffer;

            std::vector<packet::Packet*> queue;
            Mutex queueMutex;

            Mutex stateMutex;
            std::time_t lastActivity;
            crypto::AESCipher* cipher;
            entity::Player* player;

            bool performLoginSequence();
            bool recvPacket(std::vector<unsigned char> &);
    };
}
#endif //CONNECTION_H