#ifndef HYPER_PARAMS_H
#define HYPER_PARAMS_H

const int ARGC = 2;

const int SCENE_WINDOW_X = 640;
const int SCENE_WINDOW_Y = 480;
const int SCENE_WINDOW_FR = 100;

const float MAX_SPEED = 500.f;

const float MAX_VELOCITY_POS = 1500.f;
const float MAX_VELOCITY_NEG = -1500.f;

const float MAX_ACCELERATION_POS = 1000.f;
const float MAX_ACCELERATION_NEG = -1000.f;

const float MAX_ROTATION = 90.f;
const float MAX_ACCELERATION_ANGULAR_POS = 90.f;
const float MAX_ACCELERATION_ANGULAR_NEG = -90.f;

const float TIME_TO_REACH_TARGET_SPEED = 1.f;
const float TIME_TO_REACH_TARGET_ROTATION = 1.f;
const float TIME_TO_REACH_TARGET_VELOCITY = 1.f;

const float RADIUS_OF_ARRIVAL = 50.f;
const float RADIUS_OF_DECELERATION = 250.f;

#endif
