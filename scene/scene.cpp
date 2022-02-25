#include <SFML/Graphics.hpp>
#include "../character/character.h"
#include "scene.h"

using namespace sf;


		SceneView::SceneView(int w, int h, int fps)
		{
			// Set dimensions and fps.
			width = w;
			height = h;
			frames = fps;

			// Create empty scene.
			scene.create(VideoMode(width, height), "CSC484");
			scene.setFramerateLimit(frames);

			// Set corners.
			topLeft = Vector2f(0, 0);
			topRight = Vector2f(width, 0);
			botRight = Vector2f(width, height);
			botLeft = Vector2f(0, height);
		}

		int SceneView::getWidth() { return width; }
		int SceneView::getHeight() { return height; }
		int SceneView::getFrames() { return frames; }
		Vector2f SceneView::getTopRight() { return topRight; }
		Vector2f SceneView::getBotRight() { return botRight; }
		Vector2f SceneView::getBotLeft() { return botLeft; }
		Vector2f SceneView::getTopLeft() { return topLeft; }


		bool SceneView::at(const Vector2f &point, const Character &character) {
			return character.x() == point.x && character.y() == point.y;
		}

		bool SceneView::atCorner(const Character &character) {
			return at(topLeft, character) || at(topRight, character) || at(botRight, character) || at(botLeft, character);
		}

		bool SceneView::inTopRegion(const Character &character) {
			int x = character.x();
			int y = character.y();
			return x >= 0 && x <= width && y == 0;
		}

		bool SceneView::inRightRegion(const Character &character) {
			int x = character.x();
			int y = character.y();
			return y >= 0 && y <= height && x == width;
		}

		bool SceneView::inBotRegion(const Character &character) {
			int x = character.x();
			int y = character.y();
			return x >= 0 && x <= width && y == height;
		}

		bool SceneView::inLeftRegion(const Character &character) {
			int x = character.x();
			int y = character.y();
			return y >= 0 && y <= height && x == 0;
		}
