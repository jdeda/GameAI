#include <SFML/Graphics.hpp>
#include <string>
#include "../character/character.h"
#include "debug.h"

using namespace sf;

void debug(Vector2f v) {
	cout << v.x << " " << v.y << endl;
}

void debug(const Character& character) {
	cout << character.x() << " " << character.y() << endl;
}

void fail(string message) {
	cout << "error: " << message << endl;
	exit(1);
}

void sanity() {
	cout << "sanity" << endl;
}