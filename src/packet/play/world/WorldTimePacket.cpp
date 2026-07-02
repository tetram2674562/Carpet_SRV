#include "packet/play/world/WorldTimePacket.h"

namespace packet {
WorldTimePacket::WorldTimePacket() : time(0), timeOfDay(0) {}
void WorldTimePacket::writeData(Buffer &buffer) {
  buffer.writeByte(4);
  buffer.writeLong(timeOfDay);
  buffer.writeLong(time);
}
void WorldTimePacket::readData(Buffer &) {}
} // namespace packet