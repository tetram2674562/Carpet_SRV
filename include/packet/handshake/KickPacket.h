//
// Created by tetram26 on 31/07/25.
//


#ifndef KICKPACKET_H
#define KICKPACKET_H
#include "packet/Packet.h"
#include "utils/UTF16String.h"

namespace packet {
    class KickPacket :public Packet {
        public:
            KickPacket(const utils::UTF16String &);
            void writeData(Buffer &);
            void readData(Buffer &);
        private:
            utils::UTF16String reason;
    };
}
#endif //KICKPACKET_H