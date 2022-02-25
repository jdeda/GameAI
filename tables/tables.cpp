#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "../character/character.h"
#include "tables.h"
#include <unordered_map>

using namespace std;
using namespace sf;

OrientationTable::OrientationTable(const unordered_map<int, float>& orientations) {
	table = orientations;
}

float OrientationTable::getOldOrientation(const Character& character) const {
	return table.at(character.getID());
}

void OrientationTable::debug(const Character& character) const {
	float o = table.at(character.getID());
	cout << character.getID() << " " << o << endl;
}

PositionTable::PositionTable(const unordered_map<int, Vector2f>& positions) {
	table = positions;
}

Vector2f PositionTable::getOldPosition(const Character& character) const {
	return table.at(character.getID());
}

void PositionTable::debug(const Character& character) const {
	Vector2f p = table.at(character.getID());
	cout << character.getID() << " " << p.x << " " << p.y << endl;
}

CharacterTable::CharacterTable(const vector<Character*>& characters) {
	for (auto& character : characters) {
		CharacterTable::table.insert({ character->getID(), character });
		CharacterTable::characters.push_back(character);
	}
}