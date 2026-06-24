//
// Created by tetram26 on 31/07/25.
//
#ifndef AESCIPHER_H
#define AESCIPHER_H
#include <cstddef>
#include <openssl/types.h>
#include <vector>

namespace crypto {
class AESCipher {
public:
  AESCipher(const std::vector<unsigned char> &);

  AESCipher(const AESCipher &) = delete;
  AESCipher &operator=(const AESCipher &) = delete;
  ~AESCipher();
  void decrypt(const unsigned char *encryptedVal, unsigned char *decrpytedVal,
               size_t length) const;

  void decrypt(const std::vector<unsigned char> &,
               std::vector<unsigned char> &) const;

  void encrypt(const std::vector<unsigned char> &,
               std::vector<unsigned char> &) const;

private:
  EVP_CIPHER_CTX *decryptCtx;
  EVP_CIPHER_CTX *encryptCtx;
  std::vector<unsigned char> key;
};
} // namespace crypto
#endif // AESCIPHER_H