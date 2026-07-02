//
// Created by tetram26 on 31/07/25.
//

#ifndef KICKPACKET_H
#define KICKPACKET_H
#include "packet/Packet.h"
#include "utils/UTF16String.h"

namespace packet {
class KickPacket : public Packet {
public:
  KickPacket();
  KickPacket(const utils::UTF16String &);
  void writeData(Buffer &);
  void readData(Buffer &);
  utils::UTF16String getReason() const;

private:
  utils::UTF16String reason;
};
} // namespace packet
#endif // KICKPACKET_H