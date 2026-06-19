//
// Created by tetram26 on 31/07/25.
//
#include <openssl/rand.h>
#include <packet/login/ServerAuthDataPacket.h>
#include <stdexcept>

#include "server/VanillaMinecraftServer.h"

#include <iostream>
using namespace std;
namespace packet {

ServerAuthDataPacket::ServerAuthDataPacket() {
  std::vector<unsigned char> verifyToken(4);
  // Generate a 4-char verify token
  if (RAND_bytes(verifyToken.data(), 4) != 1) {
    throw std::runtime_error("RAND_chars failed");
  }
  this->verifyToken = verifyToken;
}

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
  buffer.writeBytes(server::VanillaMinecraftServer::getServer()
                        .getKeyPair()
                        .getPublicKeychars());
  // After you get public key chars from getPublicKeychars():
  buffer.writeBytes(verifyToken);
}

void ServerAuthDataPacket::readData(Buffer &buffer) {}

const vector<unsigned char> &ServerAuthDataPacket::getVerifyToken() const {
  return this->verifyToken;
}

} // namespace packet