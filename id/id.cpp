#include "id.h"

ID::ID() {
	id = count + 1;
	count += 1;
}

int ID::getID() const {
	return id;
}

void ID::setID(int id) {
	ID::id = id;
}

void ID::setCount(int count) {
	ID::count = count;
}