#ifndef HYPER_PARAMS_H
#define HYPER_PARAMS_H

const int ARGC = 2;

const int SCENE_WINDOW_X = 640;
const int SCENE_WINDOW_Y = 480;
const int SCENE_WINDOW_FR = 100;
const int BOUND_BUFFER = 25;

const float MAX_SPEED = 75.f;

const float MAX_VELOCITY_POS = 75.f;
const float MAX_VELOCITY_NEG = -75.f;

const float MAX_ROTATION = 200.f;

const float MAX_VELOCITY_ANGULAR_POS = 75.f;
const float MAX_VELOCITY_ANGULAR_NEG = -75.f;

const float MAX_ACCELERATION_POS = 75.f;
const float MAX_ACCELERATION_NEG = -75.f;

const float MAX_ACCELERATION_ANGULAR_POS = 270.f;
const float MAX_ACCELERATION_ANGULAR_NEG = -270.f;

const float TIME_TO_REACH_TARGET_SPEED = 0.25;
const float TIME_TO_REACH_TARGET_ROTATION = 0.25;
const float TIME_TO_REACH_TARGET_VELOCITY = 0.25;

const float RADIUS_OF_ARRIVAL = 25.f;
const float RADIUS_OF_DECELERATION = 500.f;

const float WANDER_OFFSET = 600.f;
const float WANDER_RADIUS = 200.f;
const float WANDER_RATE = 200.f;
const float WANDER_ORIENTATION = -90.f;
const float WANDER_MAX_ACCELERATION = 200.f;

#endif