#include "packet/play/player/SpawnPointPacket.h"

namespace packet {
SpawnPointPacket::SpawnPointPacket() {}
SpawnPointPacket::SpawnPointPacket(int x, int y, int z) : pos(x,y,z) {}
void SpawnPointPacket::writeData(Buffer &buffer) {
  buffer.writeByte(0x06);
  buffer.writeInt(pos.getX());
  buffer.writeInt(pos.getY());
  buffer.writeInt(pos.getZ());
}
void SpawnPointPacket::readData(Buffer & buffer) {
  pos.set(buffer.readInt(),buffer.readInt(),buffer.readInt());
}
} // packet