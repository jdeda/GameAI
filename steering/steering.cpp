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

float Orientation::getTimeToReachTargetRotation() { return this->timeToReachTargetRotation; }
float Orientation::getRadiusOfArrival() { return this->radiusOfArrival; }
float Orientation::getRadiusOfDeceleration() { return this->radiusOfDeceleration; }
float Orientation::getMaxRotation() { return this->maxRotation; }

Velocity::Velocity(const float f) {
    timeToReachTargetVelocity = f;
}

inline SteeringOutput Velocity::calculateAcceleration(const Kinematic& character, const Kinematic& target) {
    SteeringOutput output;
    output.linearAcceleration = target.linearVelocity - character.linearVelocity;
    output.linearAcceleration = output.linearAcceleration / timeToReachTargetVelocity;
    output.angularAcceleration = 0;
    return output;
}

float Velocity::getTimeToReachTargetVelocity() {
    return Velocity::timeToReachTargetVelocity;
}

inline SteeringOutput Rotation::calculateAcceleration(const Kinematic& character, const Kinematic& target) {
    SteeringOutput output;
    return output;
}

VelocityMatch::VelocityMatch(float f) : Velocity(f) {}

Arrive::Arrive(const float t, const float r1, const float r2, float s) : Position(t, r1, r2, s) {}

Align::Align(const float t, const float r1, const float r2, float m) : Orientation(t, r1, r2, m) {}

FollowPath::FollowPath(vector<Vector2f> p, float o, float curr, const float t, const float r1, const float r2, float s) :
    Arrive(t, r1, r2, s) {
    path = p;
    pathOffset = o;
    currentPathPosition = curr;
}

SteeringPath::SteeringPath(vector<Vector2f> path_) {
    path = path_;
}