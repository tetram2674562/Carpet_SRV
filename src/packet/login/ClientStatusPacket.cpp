#include "packet/login/ClientStatusPacket.h"
packet::ClientStatusPacket::ClientStatusPacket() : status(0) {}
void packet::ClientStatusPacket::writeData(Buffer & buffer) {
  buffer.writeByte(0xCD);
  buffer.writeByte(status & 255);
}
void packet::ClientStatusPacket::readData(Buffer & buffer) {
  status = buffer.readByte();
}
int packet::ClientStatusPacket::getStatus() const {
  return status;
}