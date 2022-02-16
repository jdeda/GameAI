#ifndef HYPER_PARAMS_H
#define HYPER_PARAMS_H

/** Number of arguments for program. */
const int ARGC = 2;

/* SceneView X length. */
const int SCENE_WINDOW_X = 640;

/* SceneView Y length. */
const int SCENE_WINDOW_Y = 480;

/* SceneView frame rate. */
const int SCENE_WINDOW_FR = 100;

/* Number of sprites. */
const int NUM_CHARACTERS = 4;

/* Expected speed for sprite moving in X dimensions. */
const float SPEED_X = 50.f;

/* Expected speed for sprite moving in Y dimensions. */
const float SPEED_Y = (float(SCENE_WINDOW_Y) / float(SCENE_WINDOW_X)) * float(SPEED_X);

/** Maximim possible speed for a sprite. */
const float MAX_SPEED = 2.f;

/** Maximim possible velocity for a sprite. */
const float MAX_VELOCITY_POS = 750.f;

/** Maximim possible velocity for a sprite. */
const float MAX_VELOCITY_NEG = -750.f;

/** Maximim possible velocity for a sprite. */
const float MAX_ACCELERATION_POS = 500.f;

/** Maximim possible velocity for a sprite. */
const float MAX_ACCELERATION_NEG = -500.f;

/** Estimated time to reach target speed. */
const float TIME_TO_REACH_TARGET_SPEED = 1.f;

/** Estimated time to reach target rotation. */
const float TIME_TO_REACH_TARGET_ROTATION = 1.f;

/** Estimated time to reach target velocity. */
const float TIME_TO_REACH_TARGET_VELOCITY = 1.f;

/** Defined radius for which sprite has arrived at a target. */
const float RADIUS_OF_ARRIVAL = 50.f;

/** Defined radius for which sprite must declerate towards target. */
const float RADIUS_OF_DECELERATION = 250.f;

#endif
