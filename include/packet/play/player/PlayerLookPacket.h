
#ifndef CARPET_SRV_PLAYERLOOKPACKET_H
#define CARPET_SRV_PLAYERLOOKPACKET_H
#include "packet/Packet.h"

namespace packet {
class PlayerLookPacket : public Packet {
public:
  PlayerLookPacket();
  void writeData(Buffer &) override;
  void readData(Buffer &) override;
  bool getOnGround() const;
  double getYaw() const;
  double getPitch() const;

private:
  bool onGround;
  double yaw;
  double pitch;

};
}


#endif //CARPET_SRV_PLAYERLOOKPACKET_H
