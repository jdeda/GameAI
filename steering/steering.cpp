#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <random>
#include "../kinematic/kinematic.h"
#include "../hparams/hyperparameters.h"
#include "../math/vmath.h"
#include "steeringoutput.h"
#include "steering.h"

using namespace sf;
using namespace std;

// TODO: NO DEF AT ALL?
// class SteeringBehavior {
//     public:         
//         virtual SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) = 0;
// };

// MAYBE THIS?
// SteeringBehavior::virtual SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) = 0;


       Position::Position(const float t, const float r1, const float r2, const float s) {
            timeToReachTargetSpeed = t;
            radiusOfArrival = r1;
            radiusOfDeceleration = r2;
            maxSpeed = s;
        }

        float Position::getTimeToReachTargetSpeed() { return this->timeToReachTargetSpeed; }
        float Position::getRadiusOfArrival() { return this->radiusOfArrival; }
        float Position::getRadiusOfDeceleration() { return this->radiusOfDeceleration; }
        float Position::getMaxSpeed() { return this->maxSpeed; }

        Orientation::Orientation(const float t, const float r1, const float r2, float m) {
            timeToReachTargetRotation = t;
            radiusOfArrival = r1;
            radiusOfDeceleration = r2;
            maxRotation = m;
        }

        float Orientation::mapToRange(int rotation) {
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

        float Orientation::getTimeToReachTargetRotation() { return this->timeToReachTargetRotation;}
        float Orientation::getRadiusOfArrival() { return this->radiusOfArrival;}
        float Orientation::getRadiusOfDeceleration() { return this->radiusOfDeceleration;}
        float Orientation::getMaxRotation() { return this->maxRotation;}


        Velocity::Velocity(const float f) {
            timeToReachTargetVelocity = f;
        }

        SteeringOutput Velocity::calculateAcceleration(const Kinematic& character, const Kinematic& target) {
            SteeringOutput output;
            output.linearAcceleration = target.linearVelocity - character.linearVelocity;
            output.linearAcceleration = output.linearAcceleration / timeToReachTargetVelocity;
            output.angularAcceleration = 0;
            return output;
        }

        float Velocity::getTimeToReachTargetVelocity() {
            return Velocity::timeToReachTargetVelocity;
        }


    SteeringOutput Rotation::calculateAcceleration(const Kinematic& character, const Kinematic& target) {
        SteeringOutput output;
        return output;
    }



        // TODO: How to do these?
        VelocityMatch::VelocityMatch(float f) : Velocity(f) {}

        SteeringOutput VelocityMatch::calculateAcceleration(const Kinematic& character, const Kinematic& target) {
            SteeringOutput output;
            output.linearAcceleration = target.linearVelocity - character.linearVelocity;
            output.linearAcceleration = output.linearAcceleration / this->getTimeToReachTargetVelocity();
            output.angularAcceleration = 0;
            return output;
        }


            
        // Arrive::Arrive(const float t, const float r1, const float r2, float s) : Position(t, r1, r2, s) {}


        // SteeringOutput Arrive::calculateAcceleration(const Kinematic& character, const Kinematic& target) {
            
        //     // Only use target position from target kinematic.
        //     Vector2f targetP = target.position;

        //     // Setup output.
        //     SteeringOutput output;
        //     float goalLinearSpeed = 0;
        //     Vector2f goalLinearVelocity;
            
        //     //Extract direction and distance from character to target.
        //     Vector2f direction = targetP - character.position;
        //     float distance = vmath::length(direction);
   
        //     //  Set goal speed.
        //     if (distance < this->getRadiusOfArrival()) {
        //         goalLinearSpeed = 0;
        //     }
        //     else if (distance > this->getRadiusOfDeceleration()) {
        //         goalLinearSpeed = this->getMaxSpeed();
        //     }
        //     else {
        //         goalLinearSpeed = this->getMaxSpeed() * (distance / this->getRadiusOfDeceleration());
        //     }
 
        //     // Set goal velocity in direction of character to target with goal speed.
        //     goalLinearVelocity = direction;
        //     goalLinearVelocity = vmath::normalized(goalLinearVelocity);
        //     goalLinearVelocity *= goalLinearSpeed;
        
        //     // Return. Create acceleration such that we can apply and get to goal velocity.
        //     output.linearAcceleration = goalLinearVelocity - character.linearVelocity;
        //     output.linearAcceleration /= this->getTimeToReachTargetSpeed();
        //     output.angularAcceleration = 0;

        //     return output;
        // }



        // Align::Align(const float t, const float r1, const float r2, float m) : Orientation(t, r1, r2, m) {}


        // SteeringOutput Align::calculateAcceleration(const Kinematic& character, const Kinematic& target) {

        //     // Setup output.
        //     SteeringOutput output;
        //     float goalAngularVelocity = 0;

        //     // Extract direction and distance from character to target.
        //     float angularVelocity = target.orientation - character.orientation;
        //     angularVelocity = Orientation::mapToRange(angularVelocity);
        //     float angularVelocityLength = abs(angularVelocity);      

        //     // Set rotation.
        //     if (angularVelocityLength < this->getRadiusOfArrival()) {
        //         goalAngularVelocity = 0;
        //     }
        //     else if (angularVelocityLength > this->getRadiusOfDeceleration()) {
        //         goalAngularVelocity = this->getMaxRotation();
        //     }
        //     else {
        //         goalAngularVelocity = this->getMaxRotation() * (angularVelocityLength / this->getRadiusOfDeceleration());
        //     }
            
        //     // Make into acceleration such that we can apply and get to goal velocity.
        //     output.angularAcceleration = goalAngularVelocity - character.angularVelocity;
        //     output.angularAcceleration /= this->getTimeToReachTargetRotation();
        //     output.linearAcceleration = Vector2f(0.f, 0.f);

        //     return output;
        // }