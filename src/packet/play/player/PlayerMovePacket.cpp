#include "packet/play/player/PlayerMovePacket.h"
namespace packet {
  PlayerMovePacket::PlayerMovePacket() : onGround(false) {}
  void PlayerMovePacket::writeData(Buffer & buffer) {
    buffer.writeByte(10);
    buffer.writeBool(onGround);
  }
  void PlayerMovePacket::readData(Buffer &) {}
}