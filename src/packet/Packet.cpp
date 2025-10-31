//
// Created by tetram26 on 30/07/25.
//

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <packet/Packet.h>
using namespace std;
namespace packet {
    Packet::Packet() : packetID(-1), size(0) {
    }

    Packet::~Packet() {
    }

    int Packet::getSize() {
        return -1;
    }

    int Packet::getPacketID() {
        return this->packetID;
    }
    void Packet::readData(Buffer &buffer) {}
    void Packet::writeData(Buffer &buffer) {}

}
