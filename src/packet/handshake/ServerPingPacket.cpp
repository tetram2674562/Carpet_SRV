#include "packet/handshake/ServerPingPacket.h"
//
// Created by tetram26 on 31/07/25.
//
using namespace std;

namespace packet {
    ServerPingPacket::ServerPingPacket(unsigned char ping): ping(ping) {
    }

    ServerPingPacket::ServerPingPacket() : ping(-1) {
    }

    void ServerPingPacket::writeData(Buffer &buffer) {
    }

    void ServerPingPacket::readData(Buffer &buffer) {
        if (buffer.getDataBuffer().size() > 0) {
            this->ping = buffer.readByte();
        }
    }
    unsigned char ServerPingPacket::getPing() const {
        return ping;
    }
}