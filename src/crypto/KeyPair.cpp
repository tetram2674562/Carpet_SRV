//
// Created by tetram26 on 30/07/25.
//
#include "crypto/KeyPair.h"

#include <iostream>
#include <ostream>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <stdexcept>

using namespace std;
namespace crypto {

    static void check(bool success, const char* message) {
        if (!success) throw runtime_error(message);
    }

    KeyPair::KeyPair(int bits): keypair(NULL) {

            EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
            check(ctx != NULL, "Failed to create EVP_PKEY_CTX");
            check(EVP_PKEY_keygen_init(ctx) > 0, "Failed to initialize keygen");
            check(EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) > 0, "Failed to set RSA key size");
            check(EVP_PKEY_keygen(ctx, &keypair) > 0, "RSA key generation failed");

            EVP_PKEY_CTX_free(ctx);
            check(EVP_PKEY_is_a(keypair, "RSA"), "Generated key is not RSA");
    }

    KeyPair::~KeyPair() {
        if (keypair) EVP_PKEY_free(keypair);
    }

    KeyPair::KeyPair(const KeyPair & keypair) {
        this->keypair = keypair.keypair;
    }

    KeyPair & KeyPair::operator=(const KeyPair & keypair) {
        if (this->keypair) {
            EVP_PKEY_free(this->keypair);
            this->keypair = NULL;
        }
        this->keypair = keypair.keypair;
        return *this;
    }

    vector<unsigned char> KeyPair::getPrivateKeychars() const {
        int len = i2d_PrivateKey(keypair, NULL);
        check(len > 0, "Could not get private key length");

        vector<unsigned char> out(len);
        unsigned char *ptr = out.data();
        check(i2d_PrivateKey(keypair, &ptr) > 0, "Could not write private key to DER");

        return out;
    }

    std::vector<unsigned char> KeyPair::getPublicKeychars() const {
        const int len = i2d_PUBKEY(keypair, NULL);
        check(len > 0, "Could not get public key length");

        std::vector<unsigned char> out(len);
         unsigned char * ptr = out.data();
        check(i2d_PUBKEY(keypair, &ptr) > 0, "Could not write public key to DER");

        return out;
    }

    std::string KeyPair::getPrivateKeyPEM() const {
        BIO* bio = BIO_new(BIO_s_mem());
        check(bio, "BIO_new failed");

        check(PEM_write_bio_PrivateKey(bio, keypair, NULL, NULL, 0,NULL, NULL),
              "PEM_write_bio_PrivateKey failed");

        BUF_MEM* mem = NULL;
        BIO_get_mem_ptr(bio, &mem);
        string pem(mem->data, mem->length);
        BIO_free(bio);
        return pem;
    }

    string KeyPair::getPublicKeyPEM() const {
        BIO* bio = BIO_new(BIO_s_mem());
        check(bio, "BIO_new failed");

        check(PEM_write_bio_PUBKEY(bio, keypair), "PEM_write_bio_PUBKEY failed");

        BUF_MEM* mem = NULL;
        BIO_get_mem_ptr(bio, &mem);
        string pem(mem->data, mem->length);
        BIO_free(bio);
        return pem;
    }

    std::vector<unsigned char> KeyPair::decryptWithPrivateKey(const std::vector<unsigned char>& encrypted) const {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(keypair, NULL);
        if (!ctx)
            throw std::runtime_error("Failed to create EVP_PKEY_CTX");

        if (EVP_PKEY_decrypt_init(ctx) <= 0)
            throw std::runtime_error("EVP_PKEY_decrypt_init failed");

        size_t outlen;
        if (EVP_PKEY_decrypt(ctx, NULL, &outlen,
                             reinterpret_cast<const unsigned char*>(encrypted.data()),
                             encrypted.size()) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to determine decrypted length");
                             }

        std::vector<unsigned char> decrypted(outlen);
        if (EVP_PKEY_decrypt(ctx,
                             decrypted.data(), &outlen,
                             encrypted.data(),
                             encrypted.size()) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("EVP_PKEY_decrypt failed");
                             }

        EVP_PKEY_CTX_free(ctx);
        decrypted.resize(outlen);
        return decrypted;
    }

} // namespace crypto