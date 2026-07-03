#include "packet/play/player/PlayerMovePacket.h"
namespace packet {
  PlayerMovePacket::PlayerMovePacket() : onGround(true) {}
  void PlayerMovePacket::writeData(Buffer & buffer) {
    buffer.writeByte(10);
    buffer.writeBool(onGround);
  }
  void PlayerMovePacket::readData(Buffer &) {}
  bool PlayerMovePacket::getOnGround() const {
    return this->onGround;
  }
  } // namespace packet