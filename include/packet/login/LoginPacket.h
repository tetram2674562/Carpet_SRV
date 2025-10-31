//
// Created by tetram26 on 31/07/25.
//

#ifndef LOGINPACKET_H
#define LOGINPACKET_H
#include <packet/Packet.h>

namespace packet {
    class LoginPacket: public Packet {
        public:
            void writeData(Buffer &);
            void readData(Buffer &);
    };
}

#endif //LOGINPACKET_H