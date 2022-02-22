#ifndef HYPER_PARAMS_H
#define HYPER_PARAMS_H

const int ARGC = 2;

const int SCENE_WINDOW_X = 640;
const int SCENE_WINDOW_Y = 480;
const int SCENE_WINDOW_FR = 100;
const int BOUND_BUFFER = 50;

const float MAX_SPEED = 100.f;

const float MAX_VELOCITY_POS = 75.f;
const float MAX_VELOCITY_NEG = -200.f;

const float MAX_ROTATION = 125.f;

const float MAX_VELOCITY_ANGULAR_POS = 250.f;
const float MAX_VELOCITY_ANGULAR_NEG = -250.f;

const float MAX_ACCELERATION_POS = 50.f;
const float MAX_ACCELERATION_NEG = -50.f;

const float MAX_ACCELERATION_ANGULAR_POS = 250.f;
const float MAX_ACCELERATION_ANGULAR_NEG = -250.f;

const float TIME_TO_REACH_TARGET_SPEED = 0.125;
const float TIME_TO_REACH_TARGET_ROTATION = 0.125;
const float TIME_TO_REACH_TARGET_VELOCITY = 0.5;

const float RADIUS_OF_ARRIVAL = 10.f;
const float RADIUS_OF_DECELERATION = 500.f;

const float WANDER_OFFSET = 100.f;
const float WANDER_RADIUS = 400.f;
const float WANDER_RATE = 200.f;
const float WANDER_ORIENTATION = 20.f;
const float WANDER_MAX_ACCELERATION = 100.f;

#endif
