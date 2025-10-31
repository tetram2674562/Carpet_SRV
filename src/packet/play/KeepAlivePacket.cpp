#include "packet/play/KeepAlivePacket.h"
//
// Created by tetram26 on 03/08/25.
//
namespace packet {
    void KeepAlivePacket::writeData(Buffer &buffer) {
        buffer.writeByte(0x0);
    }

    void KeepAlivePacket::readData(Buffer &buffer) {
    }
}
