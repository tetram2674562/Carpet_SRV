#include "packet/play/player/PlayerAbilitiesPacket.h"
packet::PlayerAbilitiesPacket::PlayerAbilitiesPacket()
    : invulnerable(false), flying(false), allowFlying(true),
      creativeMode(true), flySpeed(10), walkSpeed(10) {}
void packet::PlayerAbilitiesPacket::writeData(Buffer &buffer) {
  buffer.writeByte(0xCA);
  int i = 0;
  if (invulnerable)
    i |= 1;
  if (flying)
    i |= 2;
  if (allowFlying)
    i |= 4;
  if (creativeMode)
    i |= 8;

  buffer.writeByte(i);
  buffer.writeByte(static_cast<unsigned char>(flySpeed*255.f));
  buffer.writeByte(static_cast<unsigned char>(walkSpeed*255.f));

}
void packet::PlayerAbilitiesPacket::readData(Buffer &) {}