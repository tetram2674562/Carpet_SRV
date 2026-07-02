//
// Created by tetram26 on 03/08/25.
//

#ifndef KEEPALIVEPACKET_H
#define KEEPALIVEPACKET_H
#include "packet/Packet.h"

namespace packet {
class KeepAlivePacket : public Packet {
public:
  KeepAlivePacket();
  KeepAlivePacket(int milliseconds);
  void writeData(Buffer &);
  void readData(Buffer &);
  int getGarbage() const;

private:
  int garbage;
};
} // namespace packet
#endif // KEEPALIVEPACKET_H
