//
// Created by tetram26 on 01/08/25.
//

#ifndef LOCATION_H
#define LOCATION_H

namespace world {
    class Location {
        public:
            Location(double = 0., double = 0., double = 0.);
            double getX() const;
            double getY() const;
            double getZ() const;
            void add(double,double,double);

            void set(double, double, double);

        private:
            double x;
            double y;
            double z;
    };
}
#endif //LOCATION_H