//
// Created by tetram26 on 25/10/2025.
//

#include "server/ServerConfig.h"

ServerConfig::ServerConfig() : maxPlayers(20){
}

int ServerConfig::getMaxPlayers() const {
    return 20;
}
