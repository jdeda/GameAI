/**
 * Contains data structures and alogrithms supporting kinematics and steering behaviors.
 * @file kinematics.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
using namespace sf;

/**
 * TODOS
 * - support vector math <either functions or create a class>
 * - support clipping
 * - order of update clarification
 */

/** Represents steering behaviors. */
class SteeringBehavior {

};

/** Represents position kinematic steering behavior. */
class Position: SteeringBehavior {

};

/** Represents orientation kinematic steering behavior. */
class Orientation: SteeringBehavior {

};

/** Represents velocity kinematic steering behavior. */
class Velocity: SteeringBehavior {

};

/** Represents rotation kinematic steering behavior. */
class Rotation: SteeringBehavior {

};

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
     * @param steering the steering output to apply
     * @param time the change in time since last update
     */
    void update(SteeringOutput steering, float time) {
        position += velocity * time; // do these operations actually work properly? no...
        orientation += rotation * time;
        velocity += steering.linear * time;
        rotation += steering.angular * time;
    }
};