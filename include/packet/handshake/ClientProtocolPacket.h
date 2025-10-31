//
// Created by tetram26 on 31/07/25.
//


#ifndef CLIENTPROTOCOLPACKET_H
#define CLIENTPROTOCOLPACKET_H
#include "packet/Packet.h"

namespace packet {
    class ClientProtocolPacket : public Packet {
        public:
            void readData(Buffer &);
            void writeData(Buffer &);

            ClientProtocolPacket();

            int getProtocolVersion() const;
            std::string getUsername();
            std::string getServerHost();
            int getServerPort() const;
            int getSize();
        private:
            int protocolVersion;
            std::string username;
            std::string serverHost;
            int serverPort;
            int packetId;
    };
}
#endif //CLIENTPROTOCOLPACKET_H