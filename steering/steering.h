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
#include "../search/search.h"
#include "../level/location.h"
#include "steeringoutput.h"

using namespace sf;
using namespace std;

// TODO: Inline all calculate acceleration functions.

/** Interface representing steering behaviors. */
class SteeringBehavior
{
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
class Position : SteeringBehavior
{

private:
    /** Algorithm Hyperparameters. */
    float timeToReachTargetSpeed;
    float radiusOfArrival;
    float radiusOfDeceleration;
    float maxSpeed;

public:
    /**
     * @brief Construct a new Position object with all its hyperparameters.
     *
     * @param t timeToReachTargetSpeed
     * @param r1 radiusOfArrival
     * @param r2 radiusOfDeceleration
     * @param s maxSpeed
     */
    Position(const float t, const float r1, const float r2, const float s);

    float getTimeToReachTargetSpeed();
    float getRadiusOfArrival();
    float getRadiusOfDeceleration();
    float getMaxSpeed();
};

/** Abstract class representing orientation-matching steering behavior. */
class Orientation : SteeringBehavior
{

private:
    /** Algorithm Hyperparameters. */
    float timeToReachTargetRotation;
    float radiusOfArrival;
    float radiusOfDeceleration;
    float maxRotation;

public:
    /**
     * @brief Construct a new Orientation with all its hyperparameters
     *
     * @param t timeToReachTargetVelocity algorithm hyperparameter
     * @param t timeToReachTargetRotation
     * @param r1 radiusOfArrival
     * @param r2 radiusOfDeceleration
     * @param m maxRotation
     */
    Orientation(const float t, const float r1, const float r2, float m);

    /**
     * @brief Fits rotation into ranges between 180 degrees.
     *
     * @param rotation the rotation to map
     * @return float the mapped rotation
     */
    float mapToRange(int rotation);

    /** Field getters. */
    float getTimeToReachTargetRotation();
    float getRadiusOfArrival();
    float getRadiusOfDeceleration();
    float getMaxRotation();
};

/** Abstract class representing velocity-matching steering behavior. */
class Velocity : SteeringBehavior
{

private:
    /** Algorithm Hyperparameters. */
    float timeToReachTargetVelocity;

public:
    /**
     * @brief Construct a new Velocity object with all its hyperparameters
     *
     * @param f timeToReachTargetVelocity algorithm hyperparameter
     */
    Velocity(const float f);
    /** Returns variable-matching steering output relative to orientation. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target);

    /** Field getters. */
    float getTimeToReachTargetVelocity();
};

/** Abstract class representing rotation-matching steering behavior. */
class Rotation : SteeringBehavior
{

    /** Returns variable-matching steering output relative to orientation. */
    SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target);
};

/** Velocity-matching algorithm. */
class VelocityMatch : Velocity
{

public:
    /**
     * @brief Construct a new Velocity Match object with all its hyperparameters
     *
     * @param f timeToReachTargetVelocity algorithm hyperparameter
     */
    VelocityMatch(float f);

    /** VelocityMatch algorithm implementation of velocity matching. */
    inline SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {
        SteeringOutput output;
        output.linearAcceleration = target.linearVelocity - character.linearVelocity;
        output.linearAcceleration = output.linearAcceleration / this->getTimeToReachTargetVelocity();
        output.angularAcceleration = 0;
        return output;
    }
};

/** Position-matching algorithm implemented via Arrive. */
class Arrive : Position
{

public:
    /**
     * @brief Construct a new Arrive object with all its hyperparameters.
     *
     * @param t timeToReachTargetSpeed
     * @param r1 radiusOfArrival
     * @param r2 radiusOfDeceleration
     * @param s maxSpeed
     */
    Arrive(const float t, const float r1, const float r2, float s);

    /** Arrive algorithm implementation of position matching. */
    inline SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {

        // Only use target position from target kinematic.
        Vector2f targetP = target.position;

        // Setup output.
        SteeringOutput output;
        float goalLinearSpeed = 0;
        Vector2f goalLinearVelocity;

        // Extract direction and distance from character to target.
        Vector2f direction = targetP - character.position;
        float distance = vmath::length(direction);

        //  Set goal speed.
        if (distance < this->getRadiusOfArrival()) {
            goalLinearSpeed = 0;
        }
        else if (distance > this->getRadiusOfDeceleration()) {
            goalLinearSpeed = this->getMaxSpeed();
        }
        else {
            goalLinearSpeed = this->getMaxSpeed() * (distance / this->getRadiusOfDeceleration());
        }

        // Set goal velocity in direction of character to target with goal speed.
        goalLinearVelocity = direction;
        goalLinearVelocity = vmath::normalized(goalLinearVelocity);
        goalLinearVelocity *= goalLinearSpeed;

        // Return. Create acceleration such that we can apply and get to goal velocity.
        output.linearAcceleration = goalLinearVelocity - character.linearVelocity;
        output.linearAcceleration /= this->getTimeToReachTargetSpeed();
        output.angularAcceleration = 0;

        return output;
    }
};

/** Orientation-matching algorithm implemented via Orientation. */
class Align : Orientation
{

public:
    /**
     * @brief Construct a new Align with all its hyperparameters
     *
     * @param t timeToReachTargetVelocity algorithm hyperparameter
     * @param t timeToReachTargetRotation
     * @param r1 radiusOfArrival
     * @param r2 radiusOfDeceleration
     * @param m maxRotation
     */
    Align(const float t, const float r1, const float r2, float m);

    /** Wander algorithm implementation. */
    inline SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& target) {

        // Setup output.
        SteeringOutput output;
        float goalAngularVelocity = 0;

        // Extract direction and distance from character to target.
        float angularVelocity = target.orientation - character.orientation;
        angularVelocity = Orientation::mapToRange(angularVelocity);
        float angularVelocityLength = abs(angularVelocity);

        // Set rotation.
        if (angularVelocityLength < this->getRadiusOfArrival()) {
            goalAngularVelocity = 0;
        }
        else if (angularVelocityLength > this->getRadiusOfDeceleration()) {
            goalAngularVelocity = this->getMaxRotation();
        }
        else {
            goalAngularVelocity = this->getMaxRotation() * (angularVelocityLength / this->getRadiusOfDeceleration());
        }

        // Make into acceleration such that we can apply and get to goal velocity.
        output.angularAcceleration = goalAngularVelocity - character.angularVelocity;
        output.angularAcceleration /= this->getTimeToReachTargetRotation();
        output.linearAcceleration = Vector2f(0.f, 0.f);

        return output;
    }
};

class FollowPath : Arrive
{
private:
    Path path;
    float pathOffset;
    int currentPathIndex;
    float predictionTime;

public:

    FollowPath(const Path& p, float o, float idx, float pt, float t, float r1, float r2, float s);

    inline SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& notUsed) {
        Vector2f futurePosition = character.position + (character.linearVelocity * predictionTime);
        currentPathIndex = path.getIndex(futurePosition, currentPathIndex);
        int newTargetPathIndex = ((currentPathIndex +  pathOffset) >= path.size()) ? path.size() - 1 : currentPathIndex + pathOffset;
        Kinematic newTarget;
        newTarget.position = path.getPosition(newTargetPathIndex);
        if(newTargetPathIndex >= path.size()) { 
            newTargetPathIndex = path.size() - 1;
            return SteeringOutput();
        }
        return Arrive::calculateAcceleration(character, newTarget);
    }
};

#endif
