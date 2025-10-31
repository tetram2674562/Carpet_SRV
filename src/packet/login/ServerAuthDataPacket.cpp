//
// Created by tetram26 on 31/07/25.
//
#include <stdexcept>
#include <openssl/rand.h>
#include <packet/login/ServerAuthDataPacket.h>

#include "server/VanillaMinecraftServer.h"
using namespace std;
namespace packet {
    void ServerAuthDataPacket::writeData(Buffer &buffer) {

        buffer.writeByte(0xFD);
        /*// Login server Id
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<long long> dist;
        long long value = dist(gen);
        string loginServerId = std::format("{:x}", value); // hex string
        buffer.writeString(loginServerId);*/
        buffer.writeString("-");

        // Public key
        buffer.writeBytes(server::VanillaMinecraftServer::getServer().getKeyPair().getPublicKeychars());
        // After you get public key chars from getPublicKeychars():
        // Generate a 4-char verify token
        std::vector<unsigned char> verifyToken(4);
        if (RAND_bytes(&verifyToken[0], verifyToken.size()) != 1) {
            throw std::runtime_error("RAND_chars failed");
        }
        this->verifyToken = verifyToken;
        buffer.writeBytes(verifyToken);

    }

    void ServerAuthDataPacket::readData(Buffer &buffer) {
    }

    const vector<unsigned char>& ServerAuthDataPacket::getVerifyToken() const {
        return this->verifyToken;
    }


}