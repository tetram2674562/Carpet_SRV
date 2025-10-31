//
// Created by tetram26 on 31/07/25.
//


#ifndef KICKPACKET_H
#define KICKPACKET_H
#include "packet/Packet.h"

namespace packet {
    class KickPacket :public Packet {
        public:
            KickPacket(const std::basic_string<unsigned short> &);
            void writeData(Buffer &);
            void readData(Buffer &);
        private:
            std::basic_string<unsigned short> reason;
    };
}
#endif //KICKPACKET_H