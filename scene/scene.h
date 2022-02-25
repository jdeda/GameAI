#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include "../character/character.h"

using namespace sf;

/** Represents view where scene will take place. */
class SceneView
{
private:
	/** Width of SceneView. */
	int width;

	/** Height of SceneView. */
	int height;

	/** Frames per second of SceneView. */
	int frames;

	/** SceneView top left corner point. */
	Vector2f topLeft = Vector2f(0, 0);

	/** SceneView top right corner point. */
	Vector2f topRight = Vector2f(width, 0);

	/** SceneView bottom left corner point. */
	Vector2f botRight = Vector2f(width, height);

	/** SceneView bottom left corner point. */
	Vector2f botLeft = Vector2f(0, height);

public:
	/** Scene of SceneView. For now, it will be kept public. */
	RenderWindow scene;

	/** Constructs the SceneView. */
	SceneView(int w, int h, int fps);

	/** Getters. */
	int getWidth();
	int getHeight();
	int getFrames();
	Vector2f getTopRight();
	Vector2f getBotRight();
	Vector2f getBotLeft();
	Vector2f getTopLeft();

	/**
	 * Returns true if character is at a point otherwise false.
	 * @param point the point to compare sprite position to (immutable reference)
	 * @param character the character to check (immutable reference)
	 * @return true if character is at a point otherwise false.
	 */
	bool at(const Vector2f &point, const Character &character);

	bool atCorner(const Character &character);

	/**
	 * Returns true if character is in the SceneView top region otherwise false.
	 * @param character the character to check (immutable reference)
	 * @return true if character is in the top region otherwise false.
	 */
	bool inTopRegion(const Character &character);

	/**
	 * Returns true if character is in the SceneView right region otherwise false.
	 * @param character the character to check (immutable reference)
	 * @return true if character is in the right region otherwise false.
	 */
	bool inRightRegion(const Character &character);

	/**
	 * Returns true if character is in the SceneView bottom region otherwise false.
	 * @param character the character to check (immutable reference)
	 * @return true if character is in the bottom region otherwise false.
	 */
	bool inBotRegion(const Character &character);

	/**
	 * Returns true if character is in the SceneView left region otherwise false.
	 * @param character the character to check (immutable reference)
	 * @return true if character is in the left region otherwise false.
	 */
	bool inLeftRegion(const Character &character);
};
#endif
