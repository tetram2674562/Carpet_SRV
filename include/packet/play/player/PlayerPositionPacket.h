//
// Created by tetram26 on 01/08/25.
//

#ifndef PLAYERPOSITIONPACKET_H
#define PLAYERPOSITIONPACKET_H
#include "packet/Packet.h"
#include "world/Location.h"

namespace packet {
    class PlayerPositionPacket : public Packet{
        public:
            PlayerPositionPacket(const world::Location &, double, bool);
            PlayerPositionPacket();


            void readData(Buffer &);
            void writeData(Buffer &);
            double getX() const;

            double getY() const;

            double getZ() const;

            double getStance() const;

            bool isOnGround() const;

        private:
            world::Location location;
            double stance;
            bool onGround;
    };
}
#endif //PLAYERPOSITIONPACKET_H