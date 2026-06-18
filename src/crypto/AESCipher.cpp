//
// Created by tetram26 on 31/07/25.
//

#include "crypto/AESCipher.h"

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdexcept>
#include <vector>

namespace crypto {

AESCipher::AESCipher(const std::vector<unsigned char> &inputKey)
    : key(inputKey) {
  if (inputKey.size() != 16) {
    throw std::runtime_error("AES key must be 16 chars");
  }

  const unsigned char *k = inputKey.data();
  // Create encryption context
  encryptCtx = EVP_CIPHER_CTX_new();
  if (!encryptCtx)
    throw std::runtime_error("Failed to create encrypt context");
  if (EVP_EncryptInit_ex(encryptCtx, EVP_aes_128_cfb8(), nullptr, k, k) != 1)
    throw std::runtime_error("Failed to init AES encrypt");

  // Create decryption context
  decryptCtx = EVP_CIPHER_CTX_new();
  if (!decryptCtx)
    throw std::runtime_error("Failed to create decrypt context");
  if (EVP_DecryptInit_ex(decryptCtx, EVP_aes_128_cfb8(), nullptr, k, k) != 1)
    throw std::runtime_error("Failed to init AES decrypt");
}

AESCipher::AESCipher(const AESCipher &cipher) {
  this->key = cipher.key;
  const unsigned char *k = this->key.data();
  // Create encryption context
  encryptCtx = EVP_CIPHER_CTX_new();
  if (!encryptCtx)
    throw std::runtime_error("Failed to create encrypt context");
  if (EVP_EncryptInit_ex(encryptCtx, EVP_aes_128_cfb8(), nullptr, k, k) != 1)
    throw std::runtime_error("Failed to init AES encrypt");

  // Create decryption context
  decryptCtx = EVP_CIPHER_CTX_new();
  if (!decryptCtx)
    throw std::runtime_error("Failed to create decrypt context");
  if (EVP_DecryptInit_ex(decryptCtx, EVP_aes_128_cfb8(), nullptr, k, k) != 1)
    throw std::runtime_error("Failed to init AES decrypt");
}

AESCipher &AESCipher::operator=(const AESCipher &cipher) {
  this->key = cipher.key;
  if (this->decryptCtx) {
    EVP_CIPHER_CTX_free(this->decryptCtx);
    this->decryptCtx = cipher.decryptCtx;
  }
  if (this->encryptCtx) {
    EVP_CIPHER_CTX_free(this->encryptCtx);
    this->encryptCtx = cipher.encryptCtx;
  }
  return *this;
}

AESCipher::~AESCipher() {
  if (encryptCtx)
    EVP_CIPHER_CTX_free(encryptCtx);
  if (decryptCtx)
    EVP_CIPHER_CTX_free(decryptCtx);
  CRYPTO_cleanup_all_ex_data();
}

void AESCipher::decrypt(const std::vector<unsigned char> &encryptedData,
                        std::vector<unsigned char> &decryptedData) const {
  if (!decryptCtx)
    throw std::runtime_error("decryptCtx is null!");
  decryptedData.reserve(encryptedData.size() + AES_BLOCK_SIZE);
  int outlen = 0;
  if (EVP_DecryptUpdate(decryptCtx, decryptedData.data(), &outlen,
                        encryptedData.data(),
                        static_cast<int>(encryptedData.size())) != 1) {
    throw std::runtime_error("AES decryption failed");
  }

  decryptedData.resize(outlen);
}

void AESCipher::encrypt(const std::vector<unsigned char> &decryptedData,
                        std::vector<unsigned char> &encryptedData) const {
  if (!encryptCtx)
    throw std::runtime_error("encryptCtx is null!");

  encryptedData.reserve(decryptedData.size() +
                        AES_BLOCK_SIZE); // Ensure enough space
  int outlen = 0;
  if (EVP_EncryptUpdate(encryptCtx, encryptedData.data(), &outlen,
                        decryptedData.data(),
                        static_cast<int>(decryptedData.size())) != 1) {
    throw std::runtime_error("AES encryption failed");
  }

  encryptedData.resize(outlen);
}

} // namespace crypto
