#ifndef STEER_OUTPUT_H
#define STEER_OUTPUT_H


#include <SFML/Graphics.hpp>

using namespace sf;

/** Represents steering output parameters modeled by steering behavior. */
class SteeringOutput {

    public:

        /** Linear acceleration (d/t^2). */
        Vector2f linearAcceleration;

        /** Angular acceleration (theta/t^2). */
        float angularAcceleration;

         SteeringOutput();

};

#endif
