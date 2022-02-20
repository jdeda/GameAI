#ifndef HYPER_PARAMS_H
#define HYPER_PARAMS_H

const int ARGC = 2;

const int SCENE_WINDOW_X = 640;
const int SCENE_WINDOW_Y = 480;
const int SCENE_WINDOW_FR = 100;

const float MAX_SPEED = 100.f;

const float MAX_VELOCITY_POS = 1500.f;
const float MAX_VELOCITY_NEG = -3000.f;

const float MAX_ROTATION = 225.f;

const float MAX_VELOCITY_ANGULAR_POS = 180.f;
const float MAX_VELOCITY_ANGULAR_NEG = -180.f;

const float MAX_ACCELERATION_POS = 1000.f;
const float MAX_ACCELERATION_NEG = -2000.f;

const float MAX_ACCELERATION_ANGULAR_POS = 90.f;
const float MAX_ACCELERATION_ANGULAR_NEG = -90.f;

const float TIME_TO_REACH_TARGET_SPEED = 0.25;
const float TIME_TO_REACH_TARGET_ROTATION = 0.125;
const float TIME_TO_REACH_TARGET_VELOCITY = 1.f;

const float RADIUS_OF_ARRIVAL = 50.f;
const float RADIUS_OF_DECELERATION = 500.f;

const float WANDER_OFFSET = 10.f;
const float WANDER_RADIUS = 10.f;
const float WANDER_RATE = 10.f;
const float WANDER_ORIENTATION = 90.f;
const float WANDER_MAX_ACCELERATION = 10.f;

#endif
