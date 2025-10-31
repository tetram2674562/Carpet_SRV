//
// Created by tetram26 on 25/10/2025.
//

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

class ServerConfig {
    public:
        ServerConfig();
        int getMaxPlayers() const;
    private:
        int maxPlayers;
};
#endif //SERVERCONFIG_H