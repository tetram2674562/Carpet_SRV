//
// Created by tetram26 on 31/07/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "entity/Entity.h"
#include "network/Connection.h"

namespace network {
    class Connection;
}

namespace entity {
    class Player : public Entity {
        public:

            Player(int);

            virtual void update();

            void kickPlayer(const utils::UTF16String &);

            bool handleConnection();

            std::string getName() const;

            network::Connection& getConnection();

            void setUsername(const std::string &);

            std::string getLanguage() const;

            void setLanguage(const std::string &);

            int getRenderDistance() const;

            void setRenderDistance(int );

            bool getShowCape() const;

            void setShowCape(bool);

            virtual void setPosition(double, double, double);

            virtual world::Location getLocation() const;
        private:
            std::string name;
            network::Connection connection;
            std::string language;
            int renderDistance;
            bool showCape;
    };
}
#endif //PLAYER_H