#ifndef TABLES_H
#define TABLES_H

#include <SFML/Graphics.hpp>
#include "../character/character.h"
#include <unordered_map>

using namespace std;
using namespace sf;

class OrientationTable
{
	private:
		unordered_map<int, float> table;

	public:
		OrientationTable(const unordered_map<int, float>& orientations);

		float getOldOrientation(const Character &character) const;

		void debug(const Character &character) const;
};

class PositionTable
{
	private:
		unordered_map<int, Vector2f> table;

	public:
		PositionTable(const unordered_map<int, Vector2f>& positions);

		Vector2f getOldPosition(const Character &character) const;

		void debug(const Character &character) const;
};

class CharacterTable
{
	private:
		unordered_map<int, Character*> table;
		vector<Character*> characters;

	public:
		CharacterTable(const vector<Character*>& characters);

		inline PositionTable generatePositionTable() {
			unordered_map<int, Vector2f> positions;
			for(auto & character: characters) {
				positions.insert({character->getID(), character->getPosition()});
			}
			return PositionTable(positions);
		}

		inline OrientationTable generateOrientationTable() {
			unordered_map<int, float> orientations;
			for(auto & character: characters) {
				orientations.insert({character->getID(), character->getOrientation()});
			}
			return OrientationTable(orientations);
		}
};

#endif
