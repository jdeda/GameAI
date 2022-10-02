#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../debug/breadcrumbs.h"
#include "../steering/steeringoutput.h"
#include "../id/id.h"
#include "../kinematic/kinematic.h"
#include "../level/location.h"


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
	Character(vector<Crumb>* crumbs);

	/** Character's texture. */
	Texture texture;

	/** Character's sprite. */
	Sprite sprite;

	/** Character's scale (how large or small character is). */
	float scale;

	/** Breadcrumbs to render. */
	vector<Crumb>* breadcrumbs;

	/** Breadcrumb index (for rendering). */
	int crumb_idx;

	/** Breadcrumb timer for when to render breadcrumb.*/
	float crumb_drop_timer;

	/** Returns the character's x position. */
	int x() const;

	/** Returns the character's y position. */
	int y() const;

	Vector2f getPosition() const;

	Location getLocation() const;

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
	void update(const SteeringOutput& steering, const float dt, const bool clip) {
		kinematic.update(steering, dt, clip);
		sprite.setPosition(kinematic.position);
		sprite.setRotation(kinematic.orientation);

		// Update sprite.
		if (crumb_drop_timer > 0) {
			crumb_drop_timer -= 0.1f;
		}
		else {
			crumb_drop_timer = 1.f;
			breadcrumbs->at(crumb_idx).drop(kinematic.position);

			if (crumb_idx < NUM_CRUMBS - 1) {
				crumb_idx++;
			}
			else {
				crumb_idx = 0;
			}
		}
	}
	/** Returns the breadcrumbs from the character. */
	vector<Crumb>* getBreadCrumbs();

	inline void stop() {
		kinematic.angularVelocity = 0;
		kinematic.linearVelocity = Vector2f(0.f, 0.f);
	}

	inline void moveTo(const Vector2f& p) {
		kinematic.position = p;
		kinematic.linearVelocity = Vector2f(0.f, 0.f);
		kinematic.angularVelocity = 0.f;
		kinematic.orientation = 0.f;
		sprite.setPosition(p);
		sprite.setRotation(0);

		// Update sprite.
		if (crumb_drop_timer > 0) {
			crumb_drop_timer -= 0.1f;
		}
		else {
			crumb_drop_timer = 1.f;
			breadcrumbs->at(crumb_idx).drop(kinematic.position);

			if (crumb_idx < NUM_CRUMBS - 1) {
				crumb_idx++;
			}
			else {
				crumb_idx = 0;
			}
		}
	}
};

#endif
