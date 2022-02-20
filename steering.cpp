/**
 * Contains data structures and alogrithms supporting kinematics and steering behaviors.
 * @file steering.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "hyperparameters.h"
#include "vectormath.cpp"
#include <iostream>
#include <random>
using namespace sf;
using namespace std;

    random_device rd;
    mt19937 gen(rd());
    binomial_distribution<> d(5, 1.0/6.0);

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
            // Clip position in x plane.
            if(position.x >= SCENE_WINDOW_X) { position.x = SCENE_WINDOW_X; }
            if(position.x <= 0) { position.x = 0; }

            // Clip position in y plane.
            if(position.y >= SCENE_WINDOW_Y) { position.y = SCENE_WINDOW_Y;}
            if(position.y <= 0) { position.y = 0;}

            // Clip linearVelocity in x plane.
            if(linearVelocity.x >= MAX_VELOCITY_POS) { linearVelocity.x = MAX_VELOCITY_POS; }
            if(linearVelocity.x <= MAX_VELOCITY_NEG) { linearVelocity.x = MAX_VELOCITY_NEG; }

            // Clip linearVelocity in y plane.
            if(linearVelocity.y >= MAX_VELOCITY_POS) { linearVelocity.y = MAX_VELOCITY_POS; }
            if(linearVelocity.y <= MAX_VELOCITY_NEG) { linearVelocity.y = MAX_VELOCITY_NEG; }
            
            // Clip angularVelocity.
            if(angularVelocity >= MAX_VELOCITY_ANGULAR_POS) { angularVelocity = MAX_VELOCITY_ANGULAR_POS; }
            if(angularVelocity <= MAX_VELOCITY_ANGULAR_NEG) { angularVelocity = MAX_VELOCITY_ANGULAR_NEG; }
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
        inline void update(const SteeringOutput& steering, const float dt, const bool clip) {
            position += linearVelocity * dt; // do these operations actually work properly? maybe...
            orientation += angularVelocity * dt;
            linearVelocity += steering.linearAcceleration * dt;
            angularVelocity += steering.angularAcceleration * dt;
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

    private:
        float timeToReachTargetSpeed;
        float radiusOfArrival;
        float radiusOfDeceleration;
        float maxSpeed;

    public:

        /** Constructs class with all fields. */
        Position(const float t, const float r1, const float r2, const float s) {
            timeToReachTargetSpeed = t;
            radiusOfArrival = r1;
            radiusOfDeceleration = r2;
            maxSpeed = s;
        }

        /** Returns variable-matching steering output relative to position. */
        SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
            
            // Only use target position from target kinematic.
            Vector2f targetP = target.position;

            // Setup output.
            SteeringOutput output = SteeringOutput();
            float goalLinearSpeed = 0;
            Vector2f goalLinearVelocity;
            
            //Extract direction and distance from character to target.
            Vector2f direction = targetP - character.position;
            float distance = vmath::length(direction);
   
            //  Set goal speed.
            if (distance < radiusOfArrival) {
                goalLinearSpeed = 0;
            }
            else if (distance > radiusOfDeceleration) {
                goalLinearSpeed = maxSpeed;
            }
            else {
                goalLinearSpeed = maxSpeed * (distance / radiusOfDeceleration);
            }
 
            // Set goal velocity in direction of character to target with goal speed.
            goalLinearVelocity = direction;
            goalLinearVelocity = vmath::normalized(goalLinearVelocity);
            goalLinearVelocity *= goalLinearSpeed;
            // cout << goalLinearVelocity.x << " " << goalLinearVelocity.y << endl;
        
            // Return. Create acceleration such that we can apply and get to goal velocity.
            output.linearAcceleration = goalLinearVelocity - character.linearVelocity;
            output.linearAcceleration /= timeToReachTargetSpeed;
            output.angularAcceleration = 0;

            // cout << output.linearAcceleration.x << " " << output.linearAcceleration.y << endl;
            return output;
        }
};

/** Represents orientation-matching steering behavior. */
class Orientation: SteeringBehavior {

    private:

        float timeToReachTargetRotation;
        float radiusOfArrival;
        float radiusOfDeceleration;
        float maxRotation;
    
        float mapToRange(int rotation) {
            int r = rotation % 360;
            if (abs(r) <= 180) {
                return r;
            }
            else if (abs(r) > 180) {
                return 180 - r;
            }
            else {
                return 180 + r;
            }
        }

    public:

        /** Constructs class with all fields. */
        Orientation(const float t, const float r1, const float r2, float m) {
            timeToReachTargetRotation = t;
            radiusOfArrival = r1;
            radiusOfDeceleration = r2;
            maxRotation = m;
        }

        /** Returns variable-matching steering output relative to orientation. */
        SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {

            // Setup output.
            SteeringOutput output = SteeringOutput();
            float goalAngularVelocity = 0;

            // Extract direction and distance from character to target.
            float angularVelocity = target.orientation - character.orientation;
            angularVelocity = mapToRange(angularVelocity);
            float angularVelocityLength = abs(angularVelocity);
            
            // Set rotation.
            if (angularVelocityLength < radiusOfArrival) {
                goalAngularVelocity = 0;
            }
            else if (angularVelocityLength > radiusOfDeceleration) {
                goalAngularVelocity = maxRotation;
            }
            else {
                goalAngularVelocity = maxRotation * (angularVelocityLength / radiusOfDeceleration);
            }
            
            // Make into acceleration such that we can apply and get to goal velocity.
            output.angularAcceleration = goalAngularVelocity - character.angularVelocity;
            output.angularAcceleration /= timeToReachTargetRotation;
            output.linearAcceleration = Vector2f(0.f, 0.f);

            return output;
        }
};

/** Represents velocity-matching steering behavior. */
class Velocity: SteeringBehavior {

    private:
        /** Free parameter: estimation of time to reach target velocity. */
        float timeToReachTargetVelocity;

    public:
        /** Constructs the class with all fields. */
        Velocity(const float f) {
            timeToReachTargetVelocity = f;
        }

        /** Returns variable-matching steering output relative to orientation. */
        SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
            SteeringOutput output = SteeringOutput();
            output.linearAcceleration = target.linearVelocity - character.linearVelocity;
            output.linearAcceleration = output.linearAcceleration / timeToReachTargetVelocity;
            output.angularAcceleration = 0;
            return output;
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

// class Face: Align {

//      /** Returns variable-matching steering output to achieve Face. */
//     SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
//         SteeringOutput output = SteeringOutput();

//         // Calculate targe to delegate to align.
//         vector direction = target.position - character.position;

//         // Check for a zero direction, make no change if so.
//         if(vmath::length(direction) == 0) {
//             return target;
//         }

//         // Delegate to align.
//         Align.tar
//         return output;
//     }
// };

class Wander: SteeringBehavior {

    private:
        // Position positionMatcher;
        // Orientation orientationMatcher;
        float wanderOffset;
        float wanderRadius;
        float wanderRate;
        float wanderOrientation;
        float maxAcceleration;

    public:
        Wander(const float off, const float radius, const float rate, const float orient, const float accel) {
            wanderOffset = off;
            wanderRadius = radius;
            wanderRate = rate;
            wanderOrientation = orient;
            maxAcceleration = accel;
        }

    /** Returns variable-matching steering output to achieve Wander. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& notUsed) {
        SteeringOutput output = SteeringOutput();

        // Calcualte target to delegate.
        wanderOrientation += d(gen) * wanderRate;
        
        // Calculate the combined target orientation.
        float targetOrientation = wanderOrientation + character.orientation;
        
        // Calculate the center of the wander circle.
        Vector2f target = character.position + wanderOffset * vmath::asVector(character.orientation);

        // Calculate the target location.
        target += wanderRadius * vmath::asVector(targetOrientation);

        // Delegate to face.
        // output = Face.calculateAcceleration();

        // Now set the linear acceleration to be full at
        // acceleration in the direction of the orientation.
        output.linearAcceleration = maxAcceleration * vmath::asVector(character.orientation);


        return output;
    }
};