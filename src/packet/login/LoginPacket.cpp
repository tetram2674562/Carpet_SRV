//
// Created by tetram26 on 31/07/25.
//
#include <packet/login/LoginPacket.h>
using namespace std;

namespace packet {
    void LoginPacket::writeData(Buffer &buffer) {
        // Experimentation
        buffer.writeByte(0x01);
        buffer.writeInt(1); // Entity ID 1
        buffer.writeString("flat"); // flat world type
        buffer.writeByte(1); //
        buffer.writeByte(0); //
        buffer.writeByte(1); //
        buffer.writeByte(0xFF); //
        buffer.writeByte(20); //
    }

    void LoginPacket::readData(Buffer &) {
    }
}
