#ifndef CARPET_SRV_CLIENTSTATUSPACKET_H
#define CARPET_SRV_CLIENTSTATUSPACKET_H
#include "packet/Packet.h"

namespace packet {
  class ClientStatusPacket : public Packet{
  public:
    ClientStatusPacket();
    void writeData(Buffer &) override;
    void readData(Buffer &) override;
    int getStatus() const;

  private:
    int status;

  };
}

#endif //CARPET_SRV_CLIENTSTATUSPACKET_H
