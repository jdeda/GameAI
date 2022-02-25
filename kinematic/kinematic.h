#ifndef KINEMATIC_H
#define KINEMATIC_H

#include <SFML/Graphics.hpp>
#include "../hparams/hyperparameters.h"
#include "../steering/steeringoutput.h"

using namespace sf;

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

        Kinematic();


        inline void clip()
        {
            if(position.x >= SCENE_WINDOW_X - BOUND_BUFFER) { position.x = SCENE_WINDOW_X - BOUND_BUFFER; }
            if(position.x <= 0 + BOUND_BUFFER) { position.x = 0 + BOUND_BUFFER; }

            if(position.y >= SCENE_WINDOW_Y - BOUND_BUFFER) { position.y = SCENE_WINDOW_Y - BOUND_BUFFER; }
            if(position.y <= 0 + BOUND_BUFFER) { position.y = 0 + BOUND_BUFFER; }

            if(linearVelocity.x >= MAX_VELOCITY_POS) { linearVelocity.x = MAX_VELOCITY_POS; }
            if(linearVelocity.x <= MAX_VELOCITY_NEG) { linearVelocity.x = MAX_VELOCITY_NEG; }

            if(linearVelocity.y >= MAX_VELOCITY_POS) { linearVelocity.y = MAX_VELOCITY_POS; }
            if(linearVelocity.y <= MAX_VELOCITY_NEG) { linearVelocity.y = MAX_VELOCITY_NEG; }
            
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
            position += linearVelocity * dt;
            orientation += angularVelocity * dt;
            linearVelocity += steering.linearAcceleration * dt;
            angularVelocity += steering.angularAcceleration * dt;
            if (clip) { Kinematic::clip(); }
        } 

};
 
/** Generates and returns a kinematic. */
inline Kinematic computeKinematic(float dt, const Vector2f& positionOld, const Vector2f& positionNew, float orientationOld, float orientationNew) {
	Kinematic newKinematic;
    newKinematic.position = positionNew;
    newKinematic.linearVelocity = (positionNew - positionOld) / dt;
    newKinematic.orientation = orientationNew;
    newKinematic.angularVelocity = (orientationNew - orientationOld) / dt;
	return newKinematic;
}

#endif
