#ifndef CARPET_SRV_PLAYERABILITIESPACKET_H
#define CARPET_SRV_PLAYERABILITIESPACKET_H
#include "packet/Packet.h"

namespace packet {
  class PlayerAbilitiesPacket: public Packet {
public:
    PlayerAbilitiesPacket();
  void writeData(Buffer &) override;
  void readData(Buffer &) override;
  private:
    bool invulnerable;
    bool flying;
    bool allowFlying;
    bool creativeMode;
    float flySpeed;
    float walkSpeed;
};
}


#endif //CARPET_SRV_PLAYERABILITIESPACKET_H
