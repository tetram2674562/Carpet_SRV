#include "packet/play/KeepAlivePacket.h"
//
// Created by tetram26 on 03/08/25.
//
namespace packet {
KeepAlivePacket::KeepAlivePacket() : garbage(0) {}
KeepAlivePacket::KeepAlivePacket(int milliseconds) : garbage(milliseconds) {}
void KeepAlivePacket::writeData(Buffer &buffer) {
  buffer.writeByte(0x0);
  buffer.writeInt(garbage);
}

void KeepAlivePacket::readData(Buffer &buffer) {
  garbage = buffer.readInt();
}
int KeepAlivePacket::getGarbage() const {
  return garbage;
}
} // namespace packet
