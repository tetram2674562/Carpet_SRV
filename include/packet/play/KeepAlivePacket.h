//
// Created by tetram26 on 03/08/25.
//

#ifndef KEEPALIVEPACKET_H
#define KEEPALIVEPACKET_H
#include "packet/Packet.h"


namespace packet {
    class KeepAlivePacket: public Packet {
        public:
            void writeData(Buffer &);
            void readData(Buffer &);
    };
}
#endif //KEEPALIVEPACKET_H
