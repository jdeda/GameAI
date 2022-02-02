/**
 * Program animates sprites moving and across a window.
 * @file hw1_pt1.cpp
 * @author Jese Deda
 * @date 1/22/22
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

/* ContentWindow X length. */
const int CONTENT_WINDOW_X = 640;

/* ContentWindow Y length. */
const int CONTENT_WINDOW_Y = 480;

/* ContentWindow frame rate. */
const int CONTENT_WINDOW_FR = 100;

/* Expected speed for sprite moving in X dimensions. */
const float SPEED_X = 5.f;

/* Expected speed for sprite moving in Y dimensions. */
const float SPEED_Y = (float(CONTENT_WINDOW_Y) / float(CONTENT_WINDOW_X)) * float(SPEED_X);

/* Number of sprites. */
const int NUM_SPRITES = 4;

/* Debug output (prints the sprites coordinates). */
void debug(const Sprite &sprite) {
	cout << sprite.getPosition().x;
	cout << " ";
	cout << sprite.getPosition().y << endl;
}
/* Debug output (for sanity check). */
void sanity() {
	cout << "sanity" << endl;
}

/** ContentWindow top left corner point. */
const Vector2f topLeft = Vector2f(0, 0);

/** ContentWindow top right corner point. */
const Vector2f topRight = Vector2f(CONTENT_WINDOW_X, 0);

/** ContentWindow bottom left corner point. */
const Vector2f botRight = Vector2f(CONTENT_WINDOW_X, CONTENT_WINDOW_Y);

/** ContentWindow bottom left corner point. */
const Vector2f botLeft = Vector2f(0, CONTENT_WINDOW_Y);

/**
  * Returns true if sprite is at a point otherwise false.
  * @param point the point to compare sprite position to (immutable reference)
  * @param sprite the sprite to check (immutable reference)
  * @return true if sprite is at a point otherwise false.
  */
bool at(const Vector2f &point, const Sprite &sprite) {
	return sprite.getPosition().x == point.x && sprite.getPosition().y == point.y;
}

/**
  * Returns true if sprite is at a ContentWindow corner point otherwise false.
  * @param sprite the sprite to check (immutable reference)
  * @return true if sprite is at a corner point otherwise false.
  */
bool atCorner(const Sprite &sprite) {
	return at(topLeft, sprite) || at(topRight, sprite) || at(botRight, sprite) || at(botLeft, sprite);
}

/**
  * Returns true if sprite is in the ContentWindow top region otherwise false.
  * @param sprite the sprite to check (immutable reference)
  * @return true if sprite is in the top region otherwise false.
  */
bool inTopRegion(const Sprite &sprite) {
	int x = sprite.getPosition().x;
	int y = sprite.getPosition().y;
	return x >= 0 && x <= CONTENT_WINDOW_X && y == 0;
}
/**
  * Returns true if sprite is in the ContentWindow right region otherwise false.
  * @param sprite the sprite to check (immutable reference)
  * @return true if sprite is in the right region otherwise false.
  */
bool inRightRegion(const Sprite &sprite) {
	int x = sprite.getPosition().x;
	int y = sprite.getPosition().y;
	return y >= 0 && y <= CONTENT_WINDOW_Y && x == CONTENT_WINDOW_X;
}

/**
  * Returns true if sprite is in the ContentWindow bottom region otherwise false.
  * @param sprite the sprite to check (immutable reference)
  * @return true if sprite is in the bottom region otherwise false.
  */
bool inBotRegion(const Sprite &sprite) {
	int x = sprite.getPosition().x;
	int y = sprite.getPosition().y;
	return x >= 0 && x <= CONTENT_WINDOW_X && y == CONTENT_WINDOW_Y;
}

/**
  * Returns true if sprite is in the ContentWindow left region otherwise false.
  * @param sprite the sprite to check (immutable reference)
  * @return true if sprite is in the left region otherwise false.
  */
bool inLeftRegion(const Sprite &sprite) {
	int x = sprite.getPosition().x;
	int y = sprite.getPosition().y;
	return y >= 0 && y <= CONTENT_WINDOW_Y && x == 0;
}

/**
  * Moves the referenced sprite via mutation according to its ContentWindow region and trajectory.
  * @param sprite the sprite to move (mutable)
  */
void move(Sprite &sprite) {
	if (inTopRegion(sprite) && !at(topRight, sprite) && sprite.getRotation() == 0) { 
		sprite.move(SPEED_X, 0);
	}
	if (inRightRegion(sprite) && !at(botRight, sprite) && sprite.getRotation() == 90) { 
		sprite.move(0, SPEED_Y);
	}
	if (inBotRegion(sprite) && !at(botLeft, sprite) && sprite.getRotation() == 180) { 
		sprite.move(-SPEED_X, 0);
	}
	if (inLeftRegion(sprite) && !at(topLeft, sprite) && sprite.getRotation() == 270) {
		sprite.move(0, -SPEED_Y);
	}
}

/**
  * Rotates the referenced sprite via mutation respective to corner point it has reached.
  * @param sprite the sprite to move
  */
void rotate(Sprite &sprite) {
	if (at(topLeft, sprite)) {
		sprite.setRotation(0);
	}
	if (at(topRight, sprite)) {
		sprite.setRotation(90);
	}
	if (at(botRight, sprite)) {
		sprite.setRotation(180);
	}
	if (at(botLeft, sprite)) {
		sprite.setRotation(270);
	}
}

/** Represents status of sprite for assisting in when to render a sprite. */
enum SpriteStatus { waiting, running, justFinished, finished };


/**
  * Runs the program.
  * @return 0 if exit success otherwise failure.
  */
int main() {

    // Make sprites with boid texture.
	Texture texture;
	texture.loadFromFile("Assets/boid.png");
	vector<Sprite> sprites(NUM_SPRITES, Sprite(texture));
	for(auto &sprite: sprites) { sprite.setScale(0.05, 0.05); }

	// Account for sprites finishing.
	vector<SpriteStatus> spriteStatuses( NUM_SPRITES, SpriteStatus::waiting );
	spriteStatuses[0] = SpriteStatus::running;

	// Setup ContentView.
	RenderWindow ContentWindow(VideoMode(CONTENT_WINDOW_X, CONTENT_WINDOW_Y), "CSC484 HW1");
	ContentWindow.setFramerateLimit(CONTENT_WINDOW_FR);

	// Render in window.
	while (ContentWindow.isOpen())
	{
		// Handle window event.
		Event event;
		while(ContentWindow.pollEvent(event))
		{
			switch(event.type)
			{
				case Event::Closed:
					ContentWindow.close();
					break;
			}
		}
			
		// Rotate and move sprites accordingly.
		int i = 0;
		int j = 0;
		for(auto &sprite: sprites) { 

			// Only transform running sprites.
			if(spriteStatuses[i] == SpriteStatus::running) {

				// First sprite hit a corner, except for when it is starting.
				if (atCorner(sprite) && !at(topLeft, sprite)) {
					if (j + 1 <= NUM_SPRITES) {
						spriteStatuses[j + 1] = SpriteStatus::running;
					}
				}

				// Sprite has completed a loop.
				if (at(topLeft, sprite) && sprite.getRotation() == 270) {
					spriteStatuses[i] = SpriteStatus::justFinished;
					sprite.setRotation(0);

				}
				
				// Sprite has not completed a loop.
				else {
			    	rotate(sprite);
					move(sprite);
				}
			}
			i++;
			j++;
		}

		// If all sprites are finished, reset visualization pattern.
		int numFinished = 0;
		for(auto status: spriteStatuses) {
			if (status == SpriteStatus::finished) { numFinished += 1; } 
		}
		if (numFinished == NUM_SPRITES) { 
			for(auto status: spriteStatuses) { status = SpriteStatus:: waiting; }
			spriteStatuses[0] = SpriteStatus::running;
		}


		// Re-render ContentWindow.
		ContentWindow.clear(Color(255, 255, 255));
		int k = 0;
		for(auto &sprite: sprites) { 

			// Finish the justFinished sprite.
			if(spriteStatuses[k] == SpriteStatus::justFinished) {
				spriteStatuses[k] = SpriteStatus::finished;
				ContentWindow.draw(sprite); 
			}

			// Only render running sprites.
			if(spriteStatuses[k] == SpriteStatus::running) {
				ContentWindow.draw(sprite); 
			}
			k++;
		}
		ContentWindow.display();
	}

	// Exit progam.
    return 0;
}
