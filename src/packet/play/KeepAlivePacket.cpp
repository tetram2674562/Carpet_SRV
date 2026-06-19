#include "packet/play/KeepAlivePacket.h"
//
// Created by tetram26 on 03/08/25.
//
namespace packet {
KeepAlivePacket::KeepAlivePacket() : milliseconds(0) {}
KeepAlivePacket::KeepAlivePacket(int milliseconds) : milliseconds(milliseconds) {}
void KeepAlivePacket::writeData(Buffer &buffer) {
  buffer.writeByte(0x0);
  buffer.writeInt(milliseconds);
}

void KeepAlivePacket::readData(Buffer &buffer) {
  milliseconds = buffer.readInt();
}
} // namespace packet
