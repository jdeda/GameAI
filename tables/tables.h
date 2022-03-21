#ifndef TABLES_H
#define TABLES_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "../character/character.h"

using namespace std;
using namespace sf;

class OrientationTable
{
	private:
	/** Map of character's orientation indexing by the respective character's unique ID. */
	unordered_map<int, float> table;

	public:
	/**
	 * @brief Construct a new Orientation Table object
	 *
	 * @param orientations
	 */
	OrientationTable(const unordered_map<int, float>& orientations);

	/**
	 * @brief Get the Old Orientation object
	 *
	 * @param character
	 * @return float
	 */
	float getOldOrientation(const Character& character) const;

	/**
	 * @brief Display message to console about character orientation
	 *
	 * @param character the u
	 */
	void debug(const Character& character) const;
};

class PositionTable
{
	private:
	/** Map of character's position indexing by the respective character's unique ID. */
	unordered_map<int, Vector2f> table;

	public:
	/**
	 * @brief Construct a new PositionTable
	 *
	 * @param positions the positions to construct fill map with
	 */
	PositionTable(const unordered_map<int, Vector2f>& positions);

	/**
	 * @brief Get the old position of the character
	 *
	 * @param character
	 * @return Vector2f
	 */
	Vector2f getOldPosition(const Character& character) const;

	/**
	 * @brief Display message to console about character position
	 *
	 * @param character
	 */
	void debug(const Character& character) const;
};

class CharacterTable
{
	private:
	/** Map of characters indexing by their unique ID. */
	unordered_map<int, Character*> table;

	/**
	 * Vector of Character pointers, so that table can have constant access to
	 * character's kinematic data structure in order to generate a table of their
	 * positions or orientations, with intention to be used for velocity generation
	 * (need delta position and orientation to generate linear and angular velocities)
	 */
	vector<Character*> characters;

	public:
	CharacterTable(const vector<Character*>& characters);

	/**
	 * @brief Generates a position table at the instance of being called
	 *
	 * @return PositionTable consisting map of character's orientations
	 * indexable by the character's ID
	 */
	inline PositionTable generatePositionTable() {
		unordered_map<int, Vector2f> positions;
		for (auto& character : characters) {
			positions.insert({ character->getID(), character->getPosition() });
		}
		return PositionTable(positions);
	}

	/**
	 * @brief Generates a orientation table at the instance of being called
	 *
	 * @return OrientationTable table consisting map of character's orientations
	 * indexable by the character's ID
	 */
	inline OrientationTable generateOrientationTable() {
		unordered_map<int, float> orientations;
		for (auto& character : characters) {
			orientations.insert({ character->getID(), character->getOrientation() });
		}
		return OrientationTable(orientations);
	}
};

#endif
