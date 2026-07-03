#ifndef CARPET_SRV_CHATPACKET_H
#define CARPET_SRV_CHATPACKET_H
#include "packet/Packet.h"

namespace packet {
class ChatPacket: public Packet {
public:
  ChatPacket();
  ChatPacket(const utils::UTF16String& message);
  void writeData(Buffer &) override;
  void readData(Buffer &) override;

  utils::UTF16String getMessage() const;

private:
  utils::UTF16String message;
};

}


#endif //CARPET_SRV_CHATPACKET_H
