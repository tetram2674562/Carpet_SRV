//
// Created by tetram26 on 31/07/25.
//
#ifndef AESCIPHER_H
#define AESCIPHER_H
#include <cstddef>
#include <vector>
#include <openssl/types.h>

namespace crypto {
    class AESCipher {
    public:
        AESCipher(const std::vector<unsigned char>&);
        
        AESCipher(const AESCipher&);
        AESCipher& operator= (const AESCipher&);
        ~AESCipher();

        std::vector<unsigned char> decrypt(const std::vector<unsigned char> &);

        std::vector<unsigned char> encrypt(const std::vector<unsigned char> &);


    private:
        EVP_CIPHER_CTX* decryptCtx;
        EVP_CIPHER_CTX* encryptCtx;
        std::vector<unsigned char> key;
    };
}
#endif //AESCIPHER_H