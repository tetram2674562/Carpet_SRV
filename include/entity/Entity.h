//
// Created by tetram26 on 31/07/25.
//
#ifndef ENTITY_H
#define ENTITY_H
#include "world/Location.h"

namespace entity {
    class Entity {
        public:
            Entity();
            virtual ~Entity() {};
            virtual void update() = 0;
            virtual world::Location getLocation() const;
            virtual void setPosition(double, double, double);

        protected:
            world::Location location;
    };

}
#endif //ENTITY_H