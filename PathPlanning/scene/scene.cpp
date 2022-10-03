#include <SFML/Graphics.hpp>
#include "../character/character.h"
#include "scene.h"

using namespace sf;

SceneView::SceneView(int w, int h, int fps) {
	// Set dimensions and fps.
	width = w;
	height = h;
	frames = fps;

	// Create empty scene.
    scene.create(VideoMode(width, height), "GameAI - DecisionMaking");
	scene.setFramerateLimit(frames);
}

int SceneView::getWidth() { return width; }
int SceneView::getHeight() { return height; }
int SceneView::getFrames() { return frames; }
