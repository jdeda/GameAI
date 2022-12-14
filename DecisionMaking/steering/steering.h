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
        if (path.size() == 0) {
            cout << "empty path" << endl; // TODO: How is this happening?
            return SteeringOutput();
        }

        // Follow to center of last coordinate precise!
        if (currentPathIndex == path.size() - 1) {
            Kinematic newTarget;

            // If moving right or down, need to move more (sprite issues).
            auto a = path.getPathList()[path.size() - 1].getLocation();
            auto b = path.getPathList()[path.size() - 2].getLocation();
            auto d = getDirection(a, b);
            cout << "d: " << d << endl;
            if (d == 3) { // Moving right.
                newTarget.position = flip(mapToWindow(SIZE, path.getLast()));
                newTarget.position.x += 6;

            }
            else if (d == 2) { // Moving down.
                newTarget.position = flip(mapToWindow(SIZE, path.getLast()));
                newTarget.position.y += 6;
            }
            else { // Moving left or up.
                newTarget.position = flip(mapToWindow(SIZE, path.getLast()));
            }
            return Arrive::calculateAcceleration(character, newTarget);
        }
        Vector2f futurePosition = character.position + (character.linearVelocity * predictionTime);
        currentPathIndex = path.getIndex(futurePosition, currentPathIndex);
        int newTargetPathIndex = currentPathIndex == path.size() - 1 ? currentPathIndex : currentPathIndex + pathOffset;
        Kinematic newTarget;
        newTarget.position = path.getPosition(newTargetPathIndex);

        if (newTargetPathIndex >= path.size()) {
            currentPathIndex = path.size() - 1;
            SteeringOutput stopping;
            stopping.linearAcceleration = Vector2f(-1.f, -1.f);
            return stopping;
        }
        return Arrive::calculateAcceleration(character, newTarget);
    }
};

class Wander : Arrive
{

    private:
    float wanderOffset;
    float wanderRadius;
    float wanderRate;
    float wanderOrientation;
    float maxAcceleration;
    Vector2f wanderTargetPosition; // This is cheese...

    public:

    /** Constructor for  Wander. */
    inline  Wander(const float off, const float radius, const float rate, const float orient, const float accel,
        const float t, const float r1, const float r2, float s) : Arrive(t, r1, r2, s) {
        this->wanderOffset = off;
        this->wanderRadius = radius;
        this->wanderRate = rate;
        this->wanderOrientation = orient;
        this->maxAcceleration = accel;
    }

    inline  float getWanderOffset() { return this->wanderOffset; }
    inline  float getWanderRadius() { return this->wanderRadius; }
    inline  float getWanderRate() { return this->wanderRate; }
    inline  float getWanderOrientation() { return this->wanderOrientation; }
    inline  float getmaxAcceleration() { return this->maxAcceleration; }

    inline  float mapToRange(int rotation) {
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

    /** Returns variable-matching steering output to achieve Wander. */
    inline SteeringOutput calculateAcceleration(const Kinematic& character, const Kinematic& notUsed) {

        // Build random values and target position.
        SteeringOutput output;
        Kinematic target;
        float randomBinomial = ((double)rand() / (RAND_MAX));
        float randomDirection = ((double)rand() / (RAND_MAX));
        if (randomDirection < 0.5) { randomBinomial *= -1; }
        float randomOrientation = mapToRange((randomBinomial * 100));
        target.orientation = (randomBinomial * this->getWanderRate()) + randomOrientation;

        // Init target position.
        auto vec = vmath::asVector(character.orientation);
        cout << "vec: " << vec.x << " " << vec.y << endl;
        target.position.x = (character.position.x + wanderOffset) * vec.x;
        target.position.y = (character.position.y + wanderOffset) * vec.y;
        auto vecc = this->getWanderRadius() * vmath::asVector(target.orientation);
        target.position.x += vecc.x;
        target.position.y += vecc.y;

        // Target position should never be negative (either fix with abs or hparams).
        target.position.x = abs(target.position.x);
        target.position.y = abs(target.position.y);

        // Check if wander goes onto an invalid tile.
        cout << "target: " << target.position.x << " " << target.position.y << endl;
        auto temp = mapToLevel(22, 29.0909, target.position);
        if (temp == Location(-1, -1)) {
            cout << "OH NO!" << endl;
            return SteeringOutput();
        }

        // Set wander target position and arrive.
        this->wanderTargetPosition = target.position; // Set wanderTargetPosition for applying align.
        return Arrive::calculateAcceleration(character, target);
    }

    inline  Vector2f getWanderTargetPosition() { return this->wanderTargetPosition; }
    inline  void setWanderTargetPosition(const Vector2f& p) { this->wanderTargetPosition = p; }
};

#endif
