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

/* Expected speed for sprite moving in X dimensions. */
const float SPEED_X = 10.f;

/* Expected speed for sprite moving in Y dimensions. */
const float SPEED_Y = (float(SCENE_WINDOW_Y) / float(SCENE_WINDOW_X)) * float(SPEED_X);

/* Number of sprites. */
const int NUM_CHARACTERS = 4;

const float TIME_TO_TARGET_VELOCITY = 1.0;

/** Maximim possible velocity for a sprite. */
const float MAX_VELOCITY_POS = 50.f;

/** Maximim possible velocity for a sprite. */
const float MAX_VELOCITY_NEG = -50.f;

/** Maximim possible velocity for a sprite. */
const float MAX_ACCELERATION_POS = 50.f;

/** Maximim possible velocity for a sprite. */
const float MAX_ACCELERATION_NEG = -50.f;

#endif
