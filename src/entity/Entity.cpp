//
// Created by tetram26 on 31/07/25.
//
#include <entity/Entity.h>

namespace entity {
Entity::Entity() : location(0, 0, 0) {}
void Entity::update() {}
world::Location Entity::getLocation() const { return this->location; }

void Entity::setPosition(const double x, const double y, const double z) {
  this->location.set(x, y, z);
}
void Entity::setYaw(double yaw) {
  this->yaw = yaw;
}
void Entity::setPitch(double pitch) {
  this->pitch = pitch;
}
void Entity::setOnGround(bool onGround) {
  this->onGround = onGround;
}
} // namespace entity