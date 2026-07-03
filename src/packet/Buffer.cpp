//
// Created by tetram26 on 31/07/25.
//
#include "packet/Buffer.h"

#include "utils/ConsoleUtils.h"
using namespace std;
namespace packet {
Buffer::Buffer(const std::vector<unsigned char> &bytesArray)
    : data(bytesArray.begin(), bytesArray.end()), readPos(0) {}

Buffer::Buffer() : readPos(0) {}

asio::mutable_buffer Buffer::mutableBuffer() {
  const size_t READ_BUFFER_SIZE = 4096;
  if (data.capacity() < READ_BUFFER_SIZE) {
    data.reserve(READ_BUFFER_SIZE);
  }
  if (data.size() < READ_BUFFER_SIZE) {
    data.resize(READ_BUFFER_SIZE);
  }
  return asio::buffer(data.data() + readPos, data.size() - readPos);
}

asio::const_buffer Buffer::constBuffer() const {
  return asio::buffer(data.data() + readPos, data.size() - readPos);
}

std::size_t Buffer::size() const { return data.size(); }

void Buffer::resize(std::size_t n) { data.resize(n); }

void Buffer::addDatas(const std::vector<unsigned char> &data) {
  this->data.insert(this->data.end(), data.begin(), data.end());
}

void Buffer::clearBuffer() {
  this->data.clear();
  this->readPos = 0;
}

const vector<unsigned char> &Buffer::getDataBuffer() const {
  return this->data;
}

std::vector<unsigned char> &Buffer::getDataBuffer() { return data; }

/** Read a char from the buffer
 *
 * @return A char
 */
unsigned char Buffer::readByte() {
  if (readPos >= data.size())
    throw std::runtime_error("Buffer underflow");
  return data[readPos++];
}

/** Write a char into the buffer
 *
 * @param byte the byte
 */
void Buffer::writeByte(unsigned char byte) { data.push_back(byte); }

/** Read a short from the buffer
 *
 * @return A short
 */
short Buffer::readShort() {
  if (readPos + 2 > data.size())
    throw std::runtime_error("Buffer underflow");

  return static_cast<short>((static_cast<uint16_t>(data[readPos++]) << 8) |
                            (static_cast<uint16_t>(data[readPos++])));
}

/** Write a short into the buffer
 *
 * @param number A short
 */
void Buffer::writeShort(const short number) {
  this->data.push_back(static_cast<unsigned char>((number >> 8) & 0xFF));
  this->data.push_back(static_cast<unsigned char>(number & 0xFF));
}

/** Read a char from the buffer
 *
 * @return A char
 */
char Buffer::readChar() { return (char)readShort(); }

/** Write a char to the buffer
 *
 * @param c A char
 */
void Buffer::writeChar(char c) {
  this->data.push_back(0x00); // high char
  this->data.push_back(c);    // low char
}

/** Read a string from the buffer
 *
 * @return The string
 */
string Buffer::readString(int maxSize) {
  const short stringLen = this->readShort();
  string result;
  if (stringLen > maxSize) {
    utils::ConsoleUtils::getInstance().printerr(
        "Received string length longer than maximum allowed (> " +
        to_string(maxSize) + ")");
  } else if (stringLen < 0) {
    utils::ConsoleUtils::getInstance().printerr(
        "Received string length negative (< 0)");
  } else {
    result.reserve(stringLen);
    for (int i = 0; i < stringLen; i++) {
      result += readChar();
    }
  }
  return result;
}
/** Write a string to the buffer
 *
 * @param str A string
 */
void Buffer::writeString(const string &str) {
  writeShort(static_cast<short>(str.size()));
  for (const char i : str) {
    writeChar(i);
  }
}

double Buffer::readDouble() {
  if (readPos + 8 > data.size())
    throw std::runtime_error("Buffer underflow");

  uint64_t raw = 0;
  for (int i = 0; i < 8; ++i) {
    raw = (raw << 8) | data[readPos++];
  }
  double value;
  std::memcpy(&value, &raw, sizeof(double));
  return value;
}

void Buffer::writeDouble(double number) {
  uint64_t raw;
  std::memcpy(&raw, &number, sizeof(double));

  for (int i = 7; i >= 0; --i) {
    this->data.push_back(static_cast<unsigned char>((raw >> (i * 8)) & 0xFF));
  }
}

void Buffer::writeUTF16Char(unsigned short ch) {
  this->data.push_back(static_cast<unsigned char>(ch >> 8));
  this->data.push_back(static_cast<unsigned char>(ch & 0xFF));
}

void Buffer::writeUTF16String(const utils::UTF16String &str) {
  writeShort(static_cast<short>(str.size()));

  for (int i = 0; i < str.size(); i++) {
    writeUTF16Char(str[i]);
  }
}

utils::UTF16String Buffer::readUTF16String() {
  short len = readShort();

  utils::UTF16String string;
  for (int i = 0; i < len; ++i) {
    string += readUTF16Char();
  }
  return string;
}


vector<unsigned char> Buffer::readBytes() {
  const int16_t len = readShort();
  if (len < 0) {
    throw std::runtime_error("Negative char array length in packet.");
  }

  if (len > data.size()) {
    throw std::runtime_error(
        "Not enough chars left in packet to read char array.");
  }
  vector<unsigned char> result(len);
  std::memcpy(result.data(), data.data() + readPos, len);
  readPos += len;
  return result;
}

void Buffer::writeBytes(const vector<unsigned char> &bytes) {
  writeShort(static_cast<short>(bytes.size()));
  for (const unsigned char byte : bytes) {
    writeByte(byte);
  }
}
void Buffer::writeInt(const int number) {
  writeByte(static_cast<unsigned char>((number >> 24) & 0xFF));
  writeByte(static_cast<unsigned char>((number >> 16) & 0xFF));
  writeByte(static_cast<unsigned char>((number >> 8) & 0xFF));
  writeByte(static_cast<unsigned char>(number & 0xFF));
}

int Buffer::readInt() {
  return static_cast<int>((static_cast<uint32_t>(readByte()) << 24) |
                          (static_cast<uint32_t>(readByte()) << 16) |
                          (static_cast<uint32_t>(readByte()) << 8) |
                          static_cast<uint32_t>(readByte()));
}

bool Buffer::readBool() { return readByte() != 0; }
void Buffer::encrypt(const crypto::AESCipher &cipher) {
  decryptedData.clear();
  cipher.encrypt(data, decryptedData);
  data.swap(decryptedData);
}
void Buffer::decrypt(const crypto::AESCipher &cipher) {
  decryptedData.clear();
  cipher.decrypt(data, decryptedData);
  data.swap(decryptedData);
}
size_t Buffer::read_pos() const { return readPos; }
void Buffer::writeBool(bool val) {
  writeByte(val == true ? 1 : 0);
}
void Buffer::writeLong(long value) {
  writeInt(static_cast<int32_t>(value >> 32));
  writeInt(static_cast<int32_t>(value));
}
short Buffer::readUTF16Char() {
  return
       (static_cast<std::uint16_t>(data[readPos++]) << 8) |
       static_cast<std::uint16_t>(data[readPos++]);
}
double Buffer::readFloat() {
  float value;
  std::memcpy(&value, data.data() + readPos, sizeof(float));
  readPos += sizeof(float);
  return value;
}

} // namespace packet