#include <SFML/Graphics.hpp>
#include <iostream>
#include "location.h"

using namespace sf;

Location::Location(int a, int b) {
    x = a;
    y = b;
}

Location mapToLevel(int dimension, float mappingScale, const Vector2f& vector) {
	cout << "Mapping click..." << endl;
	int x = vector.x + 1;
	int y = vector.y + 1;
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			float min_x = i * mappingScale;
			float max_x = min_x + mappingScale;
			float min_y = j * mappingScale;
			float max_y = min_y + mappingScale;
			if ((x < max_x & x > min_x) && (y < max_y & y > min_y)) {
				// cout << "Mapped from: " << y << " " << y << endl;
				// cout << "Mapped to: " << j << " " << i << endl << endl;
				return Location(j, i); // Inverse as graphic representation inversed.
			}
		}
	}
	// cout << "No mapping found." << endl << endl;
	return Location(-1, -1);
}

Vector2f mapToWindow(float mappingScale, const Location& location) {
	// cout << "Mapping location..." << endl;
	Vector2f vector;
	vector.x = location.x * mappingScale;
	vector.y = location.y * mappingScale;
	// cout << "Mapped from: " << location.x << " " << location.y << endl;
	// cout << "Mapped to: " << vector.x << " " << vector.y << endl << endl;
	return vector;
}