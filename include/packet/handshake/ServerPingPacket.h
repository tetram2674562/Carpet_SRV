//
// Created by tetram26 on 31/07/25.
//


#ifndef SERVERPINGPACKET_H
#define SERVERPINGPACKET_H
#include "packet/Packet.h"


namespace packet {
    class ServerPingPacket : public Packet {
        public:
            ServerPingPacket(unsigned char);
            ServerPingPacket();
            ServerPingPacket(Buffer &);
            void readData(Buffer &);
            void writeData(Buffer &);
            unsigned char getPing() const;
        private:
            unsigned char ping;
    };
}
#endif //SERVERPINGPACKET_H