#include "packet/play/player/ChatPacket.h"

constexpr int MAX_LENGTH = 119;

packet::ChatPacket::ChatPacket() {}

packet::ChatPacket::ChatPacket(const utils::UTF16String &message): message(message) {}

void packet::ChatPacket::writeData(Buffer & buffer) {
  buffer.writeByte(3);
  buffer.writeUTF16String(message);
}
void packet::ChatPacket::readData(Buffer & buffer) {
  message = buffer.readUTF16String();
  if (message.size() > MAX_LENGTH)
    message = utils::UTF16String();
}

utils::UTF16String packet::ChatPacket::getMessage() const {
  return message;
}
