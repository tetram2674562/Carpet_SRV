//
// Created by tetram26 on 01/08/25.
//
#include <packet/play/player/PlayerPositionPacket.h>
using namespace std;

namespace packet {
PlayerPositionPacket::PlayerPositionPacket(const world::Location &location,
                                           double stance, bool on_ground)
    : location(location), yaw(0), pitch(0), stance(stance),
      onGround(on_ground) {}

PlayerPositionPacket::PlayerPositionPacket()
    : yaw(0), pitch(0), stance(0), onGround(false) {}

void PlayerPositionPacket::writeData(Buffer &buffer) {
  buffer.writeByte(0x0D);
  buffer.writeDouble(this->location.getX());
  buffer.writeDouble(this->location.getY());
  buffer.writeDouble(stance);
  buffer.writeDouble(this->location.getZ());
  buffer.writeDouble(yaw);
  buffer.writeDouble(pitch);
  buffer.writeBool(onGround);
}

void PlayerPositionPacket::readData(Buffer &buffer) {
  this->location.set(buffer.readDouble(), buffer.readDouble(),
                     buffer.readDouble());
  this->stance = buffer.readDouble();
  this->onGround = buffer.readBool();
}

double PlayerPositionPacket::getX() const { return this->location.getX(); }

double PlayerPositionPacket::getY() const { return this->location.getY(); }

double PlayerPositionPacket::getZ() const { return this->location.getZ(); }

double PlayerPositionPacket::getStance() const { return stance; }

bool PlayerPositionPacket::isOnGround() const { return onGround; }
} // namespace packet
