#include "packet/play/player/ClientInfoPacket.h"
//
// Created by tetram26 on 01/08/25.
//
using namespace std;

namespace packet {
    ClientInfoPacket::ClientInfoPacket() : renderDistance(0), chatVisible(0), chatColours(false), gameDifficulty(0),
                                           showCape(false) {
    }

    void ClientInfoPacket::writeData(Buffer &buffer) {
    }

    void ClientInfoPacket::readData(Buffer &buffer) {
        this->language = buffer.readString(7);
        this->renderDistance = static_cast<int>(buffer.readInt());
        char var = buffer.readInt();
        this->chatVisible = static_cast<bool>(var & 7);
        this->chatColours = (var & 8) == 8;
        this->gameDifficulty = static_cast<int>(buffer.readByte());
        this->showCape = buffer.readBool();
    }

    string ClientInfoPacket::getLanguage() const {
        return language;
    }

    int ClientInfoPacket::getRenderDistance() const {
        return renderDistance;
    }

    int ClientInfoPacket::getChatVisible() const {
        return chatVisible;
    }

    bool ClientInfoPacket::getChatColours() const {
        return chatColours;
    }

    int ClientInfoPacket::getGameDifficulty() const {
        return gameDifficulty;
    }

    bool ClientInfoPacket::getShowCape() const {
        return showCape;
    }
    int ClientInfoPacket::getSize() {
        return 7;
    }

}
