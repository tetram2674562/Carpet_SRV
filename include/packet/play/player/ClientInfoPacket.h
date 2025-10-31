//
// Created by tetram26 on 01/08/25.
//
#ifndef CLIENTINFOPACKET_H
#define CLIENTINFOPACKET_H
#include <string>

#include "packet/Packet.h"


namespace packet {
    class ClientInfoPacket : public Packet {
    public:
        ClientInfoPacket();
        void readData(Buffer &);
        void writeData(Buffer &);
        int getSize();

        std::string getLanguage() const;

        int getRenderDistance() const;

        int getChatVisible() const;

        bool getChatColours() const;

        int getGameDifficulty() const;

        bool getShowCape() const;

    private:
        std::string language;
        int renderDistance;
        int chatVisible;
        bool chatColours;
        int gameDifficulty;
        bool showCape;
    };
}

#endif //CLIENTINFOPACKET_H