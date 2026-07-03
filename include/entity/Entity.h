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
  virtual ~Entity()= default;
  virtual void update() = 0;
  virtual world::Location getLocation() const;
  virtual void setPosition(double, double, double);
  virtual void setYaw(double yaw);
  virtual void setPitch(double pitch);
  virtual void setOnGround(bool onGround);
protected:
  world::Location location;
  double yaw;
  double pitch;
  bool onGround;
};

} // namespace entity
#endif // ENTITY_H