#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "../character/character.h"
#include "debug.h"

using namespace sf;
using namespace std;

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