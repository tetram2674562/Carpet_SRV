//
// Created by tetram26 on 31/07/25.
//

#include "packet/handshake/ClientProtocolPacket.h"
using namespace std;

namespace packet {

    ClientProtocolPacket::ClientProtocolPacket(): protocolVersion(-1), serverPort(-1), packetId(0x02) {

    }
    int ClientProtocolPacket::getProtocolVersion() const {
        return this->protocolVersion;
    }

    string ClientProtocolPacket::getUsername() {
        return this->username;
    }

    string ClientProtocolPacket::getServerHost() {
        return this->serverHost;
    }

    int ClientProtocolPacket::getServerPort() const {
        return this->serverPort;
    }

    /** Read the data from the buffer
     *
     * @param buffer the buffer containing all the data (with the packet id already looked up)
     */
    void ClientProtocolPacket::readData(Buffer &buffer) {
        this->protocolVersion = buffer.readByte();
        this->username = buffer.readString(16);
        this->serverHost = buffer.readString(255);
        this->serverPort = buffer.readInt();
    }

    void ClientProtocolPacket::writeData(Buffer &buffer) {
    }

    int ClientProtocolPacket::getSize() {
        return 3 + 2 * this->username.size();
    }

}