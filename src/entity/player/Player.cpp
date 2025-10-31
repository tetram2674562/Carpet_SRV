//
// Created by tetram26 on 31/07/25.
//
#include "entity/player/Player.h"
#include "server/VanillaMinecraftServer.h"
using namespace std;

namespace entity {
    Player::Player(const int clientSocket): connection(clientSocket,*this),renderDistance(20),showCape(true) {
    }

    // Should be called everytick
    void entity::Player::update() {}

    void Player::kickPlayer(const basic_string<unsigned short> &reason) {
        connection.disconnect(reason);
    }

    bool Player::handleConnection() {
        // Pass the client fd
        return connection.handleConnection(*this);
    }

    void Player::setUsername(const string &username) {
        this->name = username;
    }

    std::string Player::getLanguage() const {
        return this->language;
    }

    void Player::setLanguage(const string &language) {
        this->language = language;
    }

    int Player::getRenderDistance() const {
        return renderDistance;
    }

    void Player::setRenderDistance(int render_distance) {
        renderDistance = render_distance;
    }

    bool Player::getShowCape() const {
        return showCape;
    }

    void Player::setShowCape(bool show_cape) {
        showCape = show_cape;
    }

    void Player::setPosition(double x, double y, double z) {
        this->location.set(x, y, z);
    }

    world::Location Player::getLocation() const{
        return this->location;
    }

    string Player::getName() const {
        return this->name;
    }

    network::Connection& Player::getConnection() {
        return this->connection;
    }

}