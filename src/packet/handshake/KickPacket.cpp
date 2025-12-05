//
// Created by tetram26 on 31/07/25.
//

#include "packet/handshake/KickPacket.h"
using namespace std;
namespace packet {
    KickPacket::KickPacket(const utils::UTF16String &reason) : reason(reason) {}

    void KickPacket::writeData(Buffer &buffer) {
        buffer.writeByte(0xFF);
        buffer.writeUTF16String(this->reason);
    }

    void KickPacket::readData(Buffer &buffer) {
    }
}
