//
// Created by tetram26 on 31/07/25.
//
#ifndef SERVERAUTHDATAPACKET_H
#define SERVERAUTHDATAPACKET_H
#include "network/Connection.h"

namespace packet {
class ServerAuthDataPacket : public Packet {
public:
  ServerAuthDataPacket();
  void writeData(Buffer &);
  void readData(Buffer &);

  const std::vector<unsigned char> &getVerifyToken() const;

private:
  std::vector<unsigned char> verifyToken;
};
} // namespace packet
#endif // SERVERAUTHDATAPACKET_H