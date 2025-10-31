//
// Created by tetram26 on 31/07/25.
//
#include <packet/login/SharedKeyPacket.h>
#include "server/VanillaMinecraftServer.h"

using namespace std;

namespace packet {
    void SharedKeyPacket::readData(Buffer &buffer) {
        const std::vector<unsigned char> encryptedSharedSecret = buffer.readBytes();
        const std::vector<unsigned char> encryptedVerifyToken = buffer.readBytes();

        this->sharedSecret = server::VanillaMinecraftServer::getServer().getKeyPair().decryptWithPrivateKey(
            encryptedSharedSecret);
        this->verifyToken = server::VanillaMinecraftServer::getServer().getKeyPair().decryptWithPrivateKey(
            encryptedVerifyToken);
    }

    void SharedKeyPacket::writeData(Buffer &buffer) {
        buffer.writeByte(0xFC);
        buffer.writeBytes(vector<unsigned char>());
        buffer.writeBytes(vector<unsigned char>());
    }
    const vector<unsigned char>& SharedKeyPacket::getSharedSecret() const{
        return this->sharedSecret;
    }

    const vector<unsigned char>& SharedKeyPacket::getVerifyToken() const {
        return this->verifyToken;
    }

}
