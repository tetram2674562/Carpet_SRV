//
// Created by tetram26 on 31/07/25.
//
#include <entity/Entity.h>


namespace entity {
    Entity::Entity(): location(0,0,0) {
    }
    void Entity::update() {

    }
    world::Location Entity::getLocation() const{
        return this->location;
    }

    void Entity::setPosition(const double x,const  double y,const double z) {
        this->location.set(x,y,z);
    }
}