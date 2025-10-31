#include "world/Location.h"
//
// Created by tetram26 on 01/08/25.
//
using namespace std;

namespace world {
    Location::Location(const double x, const double y, const double z): x(x), y(y), z(z) {}
    double Location::getX() const {
        return this->x;
    }
    double Location::getY() const {
        return this->y;
    }
    double Location::getZ() const {
        return this->z;
    }
    void Location::add(const double x, const double y, const double z) {
        this->x += x;
        this->y += y;
        this->z += z;
    }

    void Location::set(const double x, const double y, const double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }


}