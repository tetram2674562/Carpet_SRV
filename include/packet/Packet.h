//
// Created by tetram26 on 30/07/25.
//


#ifndef PACKET_H
#define PACKET_H
#include <cstddef>

#include "Buffer.h"
namespace packet {
    class Packet {
        public:
            Packet();
            virtual int getSize();
            virtual ~Packet();

            virtual int getPacketID();

            virtual void writeData(Buffer &) = 0;
            virtual void readData(Buffer &) = 0;
        protected:
            int packetID;
            int size;
    };
}
#endif //PACKET_H