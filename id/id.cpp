#include "id.h"

ID::ID() {
	id = count + 1;
	count += 1;
}

int ID::getID() const {
	return id;
}