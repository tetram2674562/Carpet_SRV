//
// Created by tetram26 on 31/07/25.
//

#include "crypto/AESCipher.h"

#include <stdexcept>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>

namespace crypto {


    AESCipher::AESCipher(const std::vector<unsigned char> &inputKey) : key(inputKey){
        if (inputKey.size() != 16) {
            throw std::runtime_error("AES key must be 16 chars");
        }

        const unsigned char* k = inputKey.data();
        // Create encryption context
        encryptCtx = EVP_CIPHER_CTX_new();
        if (!encryptCtx) throw std::runtime_error("Failed to create encrypt context");
        if (EVP_EncryptInit_ex(encryptCtx, EVP_aes_128_cfb8(), NULL, k, k) != 1)
            throw std::runtime_error("Failed to init AES encrypt");

        // Create decryption context
        decryptCtx = EVP_CIPHER_CTX_new();
        if (!decryptCtx) throw std::runtime_error("Failed to create decrypt context");
        if (EVP_DecryptInit_ex(decryptCtx, EVP_aes_128_cfb8(), NULL, k, k) != 1)
            throw std::runtime_error("Failed to init AES decrypt");
    }

    AESCipher::AESCipher(const AESCipher & cipher) {
        this->key = cipher.key;
        const unsigned char* k = this->key.data();
        // Create encryption context
        encryptCtx = EVP_CIPHER_CTX_new();
        if (!encryptCtx) throw std::runtime_error("Failed to create encrypt context");
        if (EVP_EncryptInit_ex(encryptCtx, EVP_aes_128_cfb8(), NULL, k, k) != 1)
            throw std::runtime_error("Failed to init AES encrypt");

        // Create decryption context
        decryptCtx = EVP_CIPHER_CTX_new();
        if (!decryptCtx) throw std::runtime_error("Failed to create decrypt context");
        if (EVP_DecryptInit_ex(decryptCtx, EVP_aes_128_cfb8(), NULL, k, k) != 1)
            throw std::runtime_error("Failed to init AES decrypt");
    }

    AESCipher& AESCipher::operator=(const AESCipher& cipher) {
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
        if (encryptCtx) EVP_CIPHER_CTX_free(encryptCtx);
        if (decryptCtx) EVP_CIPHER_CTX_free(decryptCtx);
        CRYPTO_cleanup_all_ex_data();
    }

    std::vector<unsigned char> AESCipher::decrypt(const std::vector<unsigned char>& data) {
        if (!decryptCtx) throw std::runtime_error("decryptCtx is null!");

        std::vector<unsigned char> out(data.size() + AES_BLOCK_SIZE); // Ensure enough space
        int outlen = 0;
        if (EVP_DecryptUpdate(decryptCtx,
                              out.data(), &outlen,
                              data.data(),
                              static_cast<int>(data.size())) != 1) {
            throw std::runtime_error("AES decryption failed");
                              }

        out.resize(outlen);
        return out;
    }

    std::vector<unsigned char> AESCipher::encrypt(const std::vector<unsigned char>& data) {
        if (!encryptCtx) throw std::runtime_error("encryptCtx is null!");

        std::vector<unsigned char> out(data.size() + AES_BLOCK_SIZE); // Ensure enough space
        int outlen = 0;
        if (EVP_EncryptUpdate(encryptCtx,
                              out.data(), &outlen,
                              data.data(),
                              static_cast<int>(data.size())) != 1) {
            throw std::runtime_error("AES encryption failed");
                              }

        out.resize(outlen);
        return out;
    }

}
