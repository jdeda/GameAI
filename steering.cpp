/**
 * Contains data structures and alogrithms supporting kinematics and steering behaviors.
 * @file steering.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "hyperparameters.h"
using namespace sf;

/** Represents steering output parameters modeled by steering behavior. */
class SteeringOutput {

public:

    /** Linear acceleration (d/t^2). */
    Vector2f linearAcceleration;

    /** Angular acceleration (theta/t^2). */
    float angularAcceleration;
};

/** Represents kinematic data for steering behaviors. Units are not defined. */
class Kinematic {

public:
    // TODO: these variables should be private with getters, and can only be set via an update.

    /** Current point in space (x, y). */
    Vector2f position;

    /** Current point in angular space alias angle (theta). */
    float orientation;

    /** Current linear velocity (d/t). */
    Vector2f linearVelocity;

    /** Current angular velocity (theta/t). */
    float angularVelocity;

    // TODO: Store clipping parameters somewhere as static constant variables.
    inline void clip()
    {
        if(position.x >= SCENE_WINDOW_X) {
            position.x = SCENE_WINDOW_X;
        }
        if(position.x < 0) {
            position.x = 0;
        }
        if(position.y >= SCENE_WINDOW_Y) {
            position.y = SCENE_WINDOW_Y;
        }
        if(position.y < 0) {
            position.y = 0;
        }
    }

    /**
     * Updates all kinematic data given steering input and change in time
     * using the Netwon-Euler 1 integration update algorithm. Order of update matters:
     * apply steering output last to ensure character gets update from last time around 
     * update (no retroactive update) and optionally clip.
     * @param steering the steering output to apply (immutable)
     * @param time the change in time since last update (immutable)
     * @param clip if true clip, else no do not clip (immutable)
     */
    inline void update(const SteeringOutput& steering, const float time, const bool clip) {
        position += linearVelocity * time; // do these operations actually work properly? maybe...
        orientation += angularVelocity * time;
        linearVelocity += steering.linearAcceleration * time;
        angularVelocity += steering.angularAcceleration * time;
        if (clip) { this->clip(); }
    } 
};
 
inline Kinematic computeKinematic(float dt, const Vector2f& positionOld, const Vector2f& positionNew, float orientationOld, float orientationNew) {
	Kinematic newKinematic;
    newKinematic.position = positionNew;
    newKinematic.linearVelocity = (positionNew - positionOld) / dt;
    newKinematic.orientation = orientationNew;
    newKinematic.angularVelocity = (orientationNew - orientationOld) / dt;
	return newKinematic;
}

/** Represents steering behaviors. */
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

/** Represents position-matching steering behavior. */
class Position: SteeringBehavior {

    /** Returns variable-matching steering output relative to position. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
        SteeringOutput output = SteeringOutput();
        return output;
    }
};

/** Represents orientation-matching steering behavior. */
class Orientation: SteeringBehavior {

    /** Returns variable-matching steering output relative to orientation. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
        SteeringOutput output = SteeringOutput();
        return output;
    }
};

/** Represents velocity-matching steering behavior. */
class Velocity: SteeringBehavior {

private:

    /** Free parameter: estimation of time to reach target velocity. */
    float timeToTargeVelocity;

public:

    Velocity(float f) {
        timeToTargeVelocity = f;
    }

    /** Returns variable-matching steering output relative to orientation. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
        SteeringOutput output = SteeringOutput();
        output.linearAcceleration = target.linearVelocity - character.linearVelocity;
        output.linearAcceleration /= timeToTargeVelocity;
        output.angularAcceleration = 0;
        return output;
    }

    float getTimeToTargeVelocity() {
        return timeToTargeVelocity;
    }
};

/** Represents rotation-matching steering behavior. */
class Rotation: SteeringBehavior {

    /** Returns variable-matching steering output relative to orientation. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
        SteeringOutput output = SteeringOutput();
        return output;
    }
};