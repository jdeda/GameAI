#ifndef DEBUG_H
#define DEBUG_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../character/character.h"

using namespace sf;

/* Debug output (prints the vector coordinates). */
void debug(Vector2f v);

/* Debug output (prints the sprites coordinates). */
void debug(const Character &character);

/** Exits program and displays message to console*/
void fail(string message);


/* Debug output (for sanity check). */
void sanity();

#endif
