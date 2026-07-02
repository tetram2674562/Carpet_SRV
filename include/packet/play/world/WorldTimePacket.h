#ifndef CARPET_SRV_WORLDTIMEPACKET_H
#define CARPET_SRV_WORLDTIMEPACKET_H
#include "packet/Packet.h"

namespace packet {

class WorldTimePacket: public Packet {
public:
  WorldTimePacket();
  void writeData(Buffer &) override;
  void readData(Buffer &) override;
private:
  long time;
  long timeOfDay;
};

}


#endif //CARPET_SRV_WORLDTIMEPACKET_H
