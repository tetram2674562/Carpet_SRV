#include "packet/play/player/PlayerLookPacket.h"


packet::PlayerLookPacket::PlayerLookPacket()
    : onGround(false), yaw(0), pitch(0) {}
void packet::PlayerLookPacket::writeData(Buffer &) {}
void packet::PlayerLookPacket::readData(Buffer &buffer) {
  yaw = buffer.readFloat();
  pitch = buffer.readFloat();
  onGround = buffer.readBool();
}
bool packet::PlayerLookPacket::getOnGround() const { return onGround; }
double packet::PlayerLookPacket::getYaw() const { return yaw; }
double packet::PlayerLookPacket::getPitch() const { return pitch; }