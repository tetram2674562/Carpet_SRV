#ifndef CARPET_SRV_PLAYERMOVEPACKET_H
#define CARPET_SRV_PLAYERMOVEPACKET_H
#include "packet/Packet.h"
namespace packet {
class PlayerMovePacket : public Packet {
public:
  PlayerMovePacket();
  void writeData(Buffer &) override;
  void readData(Buffer &) override;
  bool getOnGround() const;

private:
  bool onGround;
};
}



#endif //CARPET_SRV_PLAYERMOVEPACKET_H
