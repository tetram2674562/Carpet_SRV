//
// Created by tetram26 on 30/07/25.
//

#ifndef KEYPAIR_H
#define KEYPAIR_H
#include <string>
#include <vector>
#include <openssl/rsa.h>
namespace crypto {
    class KeyPair {
    public:
        KeyPair(int bits = 2048);

        ~KeyPair();

        std::vector<unsigned char> getPrivateKeychars() const; 
        std::vector<unsigned char> getPublicKeychars() const; 

        std::string getPrivateKeyPEM() const;
        std::string getPublicKeyPEM() const;

        std::vector<unsigned char> decryptWithPrivateKey(const std::vector<unsigned char> &) const;

    private:
        EVP_PKEY* keypair;

        // NO COPY ALLOWED AS IT COULD MESS WITH THE SERVER.
        KeyPair(const KeyPair&);
        KeyPair& operator=(const KeyPair&);

    };
}
#endif //KEYPAIR_H