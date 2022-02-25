#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../debug/breadcrumbs.h"
#include "../steering/steeringoutput.h"
#include "../id/id.h"
#include "../kinematic/kinematic.h"

using namespace std;
using namespace sf;

/** Character is a sprite with additional attributes and behaviors. */
class Character
{

private:
	/** Character's kinematic data. */
	Kinematic kinematic;

	/** Character's unique ID. */
	ID id;

public:
	/** Constructs a Character with a unique ID. */
	Character(vector<Crumb> *crumbs);

	/** Character's texture. */
	Texture texture;

	/** Character's sprite. */
	Sprite sprite;

	/** Character's scale (how large or small character is). */
	float scale;

	/** Breadcrumbs to render. */
	vector<Crumb> *breadcrumbs;

	/** Breadcrumb index (for rendering). */
	int crumb_idx;

	/** Breadcrumb timer for when to render breadcrumb.*/
	float crumb_drop_timer;

	/** Returns the character's x position. */
	int x() const;

	/** Returns the character's y position. */
	int y() const;

	Vector2f getPosition() const;

	float getOrientation() const;

	/** Returns the character's id. */
	int getID() const;

	/** Returns the character's kinematic. */
	Kinematic getKinematic();

	/** Sets the character's kinematic.*/
	void setKinematic(Kinematic kinematic);

	/**
	 * Mutates the character by updating its kinematic via parameters, followed by moving
	 * the sprite by setting its to the new kinematic position (and orientation) and updates
	 * bread crumbs.
	 */
	void update(const SteeringOutput &steering, const float dt, const bool clip);

	/** Returns the breadcrumbs from the character. */
	vector<Crumb> *getBreadCrumbs();
};

#endif
