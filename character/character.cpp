#include <SFML/Graphics.hpp>
#include <vector>
#include "../debug/breadcrumbs.h"
#include "../steering/steeringoutput.h"
#include "../id/id.h"
#include "../kinematic/kinematic.h"
#include "../level/location.h"
#include "character.h"

using namespace std;
using namespace sf;

Character::Character(vector<Crumb>* crumbs) {
	id = ID();
	crumb_idx == 100.f;
	crumb_idx = 1;
	breadcrumbs = crumbs;
}

int Character::x() const {
	return sprite.getPosition().x;
}

int Character::y() const {
	return sprite.getPosition().y;
}

Vector2f Character::getPosition() const {
	return sprite.getPosition();
}

Location Character::getLocation() const {
	return mapToLevel(MAZE_X, SIZE, sprite.getPosition());
}

float Character::getOrientation() const {
	return sprite.getRotation();
}

int Character::getID() const {
	return id.getID();
}

Kinematic Character::getKinematic() {
	return kinematic;
}

void Character::setKinematic(Kinematic kinematic) {
	this->kinematic = kinematic;
}

vector<Crumb>* Character::getBreadCrumbs() {
	return breadcrumbs;
}