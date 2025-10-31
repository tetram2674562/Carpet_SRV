//
// Created by tetram26 on 31/07/25.
//


#ifndef SHAREDKEYPACKET_H
#define SHAREDKEYPACKET_H
#include "network/Connection.h"

namespace packet {
    class SharedKeyPacket: public Packet {
        public:
            void readData(Buffer &);
            void writeData(Buffer &);

            const std::vector<unsigned char>& getSharedSecret() const;

            const std::vector<unsigned char>& getVerifyToken() const;
        private:
            std::vector<unsigned char> sharedSecret;
            std::vector<unsigned char> verifyToken;
    };
}
#endif //SHAREDKEYPACKET_H