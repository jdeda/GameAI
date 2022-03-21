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

	public:
	/** Scene of SceneView. For now, it will be kept public. */
	RenderWindow scene;

	/** Constructs the SceneView. */
	SceneView(int w, int h, int fps);

	/** Getters. */
	int getWidth();
	int getHeight();
	int getFrames();
};
#endif
