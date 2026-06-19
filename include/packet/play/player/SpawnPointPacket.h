#ifndef CARPET_SRV_SPAWNPOINTPACKET_H
#define CARPET_SRV_SPAWNPOINTPACKET_H
#include "packet/Packet.h"
#include "world/Location.h"
namespace packet{

class SpawnPointPacket : public Packet {
public:
  SpawnPointPacket();
  SpawnPointPacket(int x,int y, int z);
  void writeData(Buffer &) override;
  void readData(Buffer &) override;
private:
  world::Location pos;
};

}
#endif //CARPET_SRV_SPAWNPOINTPACKET_H
