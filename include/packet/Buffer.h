//
// Created by tetram26 on 31/07/25.
//

#ifndef BUFFER_H
#define BUFFER_H
#include "asio.hpp"
#include "crypto/AESCipher.h"
#include "utils/UTF16String.h"
#include <string>
#include <vector>

namespace packet {
class Buffer {
public:
  Buffer(const std::vector<unsigned char> &);
  Buffer();

  asio::mutable_buffer mutableBuffer();
  asio::const_buffer constBuffer() const;
  std::size_t size() const;
  void resize(std::size_t n);

  void clearBuffer();
  [[nodiscard]] const std::vector<unsigned char> &getDataBuffer() const;
  std::vector<unsigned char> &getDataBuffer();
  void addDatas(const std::vector<unsigned char> &);

  // Read write functions
  unsigned char readByte();
  void writeByte(unsigned char);

  short readShort();
  void writeShort(short);

  char readChar();
  void writeChar(char);

  std::string readString(int);
  void writeString(const std::string &);

  double readDouble();
  void writeDouble(double);

  void writeUTF16Char(unsigned short);
  void writeUTF16String(const utils::UTF16String &);

  std::vector<unsigned char> readBytes();
  void writeBytes(const std::vector<unsigned char> &);

  void writeInt(int);

  int readInt();

  bool readBool();

  void encrypt(const crypto::AESCipher &);
  void decrypt(const crypto::AESCipher &);

  size_t read_pos() const;
  void writeBool(bool val);

private:
  std::vector<unsigned char> data;
  std::vector<unsigned char> decryptedData;
  size_t readPos;
};
} // namespace packet
#endif // BUFFER_H