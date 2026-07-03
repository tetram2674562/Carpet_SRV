//
// Created by tetram26 on 31/07/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "entity/Entity.h"
#include "network/Connection.h"
#include "packet/play/player/PlayerMovePacket.h"

namespace network {
class Connection;
}

namespace entity {
class Player : public Entity {
public:
  Player(const network::Connection::pointer &);
  virtual void update();

  void kickPlayer(const utils::UTF16String &);

  void handleConnection();
  network::Connection &getConnection();

  std::string getName() const;
  void setUsername(const std::string &);

  std::string getLanguage() const;
  void setLanguage(const std::string &);

  int getRenderDistance() const;
  void setRenderDistance(int);

  bool getShowCape() const;
  void setShowCape(bool);

  void setPosition(double, double, double);
  world::Location getLocation() const;

private:
  std::string name;
  network::Connection::pointer connection;
  std::string language;
  int renderDistance;
  bool showCape;
};
} // namespace entity
#endif // PLAYER_H