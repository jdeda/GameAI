/**
 * Contains data structures and alogrithms supporting kinematics and steering behaviors.
 * @file steering.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace sf;

/**
 * TODOS
 * - support vector math <either functions or create a class> <maybe it works?>
 * - support clipping
 * - order of update clarification
 * - implement variable matching algorithms
 */

/** Represents steering output parameters modeled by steering behavior. */
class SteeringOutput {

public:

    /** Linear acceleration (d/t^2). */
    Vector2f linear;

    /** Angular acceleration (theta/t^2). */
    float angular;
};

/** Represents kinematic data for steering behaviors. Units are not defined. */
class Kinematic {

public:

    /** Current point in space (x, y). */
    Vector2f position;

    /** Current point in angular space alias angle (theta). */
    float orientation;

    /** Current linear velocity (d/t). */
    Vector2f velocity;

    /** Current angular velocity (theta/t). */
    float rotation;

    /**
     * Updates all kinematic data given steering input and change in time
     * using the Netwon-Euler 1 integration update algorithm. Order of update matters:
     * apply steering output last to ensure character gets update from last time around 
     * update (no retroactive update) and optionally clip.
     * @param steering the steering output to apply (immutable)
     * @param time the change in time since last update (immutable)
     */
    void update(const SteeringOutput& steering, const float time) {
        position += velocity * time; // do these operations actually work properly? maybe...
        orientation += rotation * time;
        velocity += steering.linear * time;
        rotation += steering.angular * time;
    }
};
 

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

/** Clips a given steering output according to free parameters. */
struct Clip {

     /** Free parameter: maximal possible acceleration. */
    float maxAcceleration;

    /**
     * Returns a clipped version of the given steering output.
     * @param output (immutable)
     * @return clipped steering output
     */
    SteeringOutput clip(const SteeringOutput& output) {
        SteeringOutput clipped = SteeringOutput();
        return clipped;
    }
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
        output.linear = target.velocity - character.velocity;
        output.linear /= timeToTargeVelocity;
        output.angular = 0;
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