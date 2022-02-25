#ifndef STEER_H
#define STEER_H

/**
 * Contains data structures and alogrithms supporting kinematics and steering behaviors.
 * @file steering.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <random>
#include "../kinematic/kinematic.h"
#include "../hparams/hyperparameters.h"
#include "../math/vmath.h"
#include "steeringoutput.h"

using namespace sf;
using namespace std;


/** Interface representing steering behaviors. */
class SteeringBehavior {
    public:         

        /**
         * Calculates accelerations for variable matching of relative variable (when implemented concretely).
         * @param character the character kinematic (immutable)
         * @param target the target kinematic (immutable)
         * @return steering output
         */
        virtual SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) = 0;
};

/** Abstract class representing position-matching steering behavior. */
class Position: SteeringBehavior {

    private:
        float timeToReachTargetSpeed;
        float radiusOfArrival;
        float radiusOfDeceleration;
        float maxSpeed;

    public:

        /** Constructs class with all fields. */
        Position(const float t, const float r1, const float r2, const float s);

        float getTimeToReachTargetSpeed();
        float getRadiusOfArrival();
        float getRadiusOfDeceleration();
        float getMaxSpeed();
};

/** Abstract class representing orientation-matching steering behavior. */
class Orientation: SteeringBehavior {

    private:

        float timeToReachTargetRotation;
        float radiusOfArrival;
        float radiusOfDeceleration;
        float maxRotation;

    public:

        /** Constructs class with all fields. */
        Orientation(const float t, const float r1, const float r2, float m);

        // Fits rotation into ranges between 180 degrees.
        float mapToRange(int rotation);

        float getTimeToReachTargetRotation();
        float getRadiusOfArrival();
        float getRadiusOfDeceleration();
        float getMaxRotation();
};

/** Abstract class representing velocity-matching steering behavior. */
class Velocity: SteeringBehavior {

    private:
        /** Free parameter: estimation of time to reach target velocity. */
        float timeToReachTargetVelocity;

    public:

        /** Constructs the class with all fields. */
        Velocity(const float f);

        /** Returns variable-matching steering output relative to orientation. */
        SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target);

        float getTimeToReachTargetVelocity();
};

/** Abstract class representing rotation-matching steering behavior. */
class Rotation: SteeringBehavior {

    /** Returns variable-matching steering output relative to orientation. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target);
};

/** Velocity-matching algorithm. */
class VelocityMatch: Velocity {

    public:

        VelocityMatch(float f);

        /** VelocityMatch algorithm implementation of velocity matching. */
        SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target);
};

// /** Position-matching algorithm implemented via Arrive. */
// class Arrive: Position {

//     public:
            
//         Arrive(const float t, const float r1, const float r2, float s) : Position(t, r1, r2, s) {}

//         /** Arrive algorithm implementation of position matching. */
//         SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target);
// };

// /** Orientation-matching algorithm implemented via Orientation. */
// class Align: Orientation {

//     public:


//         Align(const float t, const float r1, const float r2, float m) : Orientation(t, r1, r2, m) {}

//         /** Wander algorithm implementation. */
//         SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target);
// };

#endif


