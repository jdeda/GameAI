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

/* SceneView X length. */
const int SCENE_WINDOW_X = 640;

/* SceneView Y length. */
const int SCENE_WINDOW_Y = 480;

/* SceneView frame rate. */
const int SCENE_WINDOW_FR = 100;

/* Expected speed for sprite moving in X dimensions. */
const float SPEED_X = 5.f;

/* Expected speed for sprite moving in Y dimensions. */
const float SPEED_Y = (float(SCENE_WINDOW_Y) / float(SCENE_WINDOW_X)) * float(SPEED_X);

/* Number of sprites. */
const int NUM_SPRITES = 4;

/* Debug output (prints the sprites coordinates). */
void debug(const Sprite &sprite)
{
	cout << sprite.getPosition().x;
	cout << " ";
	cout << sprite.getPosition().y << endl;
}

/* Debug output (for sanity check). */
void sanity()
{
	cout << "sanity" << endl;
}

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
	SceneView(int w, int h, int fps)
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

	/** Getters. */
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getFrames() { return frames; }
	Vector2f getTopRight() { return topRight; }
	Vector2f getBotRight() { return botRight; }
	Vector2f getBotLeft() { return botLeft; }
	Vector2f getTopLeft() { return topLeft; }

	/**
 	* Returns true if sprite is at a point otherwise false.
  	* @param point the point to compare sprite position to (immutable reference)
  	* @param sprite the sprite to check (immutable reference)
  	* @return true if sprite is at a point otherwise false.
  	*/
	bool at(const Vector2f &point, const Sprite &sprite)
	{
		return sprite.getPosition().x == point.x && sprite.getPosition().y == point.y;
	}

	/**
  	* Returns true if sprite is at a SceneView corner point otherwise false.
	* @param sprite the sprite to check (immutable reference)
	* @return true if sprite is at a corner point otherwise false.
	*/
	bool atCorner(const Sprite &sprite)
	{
		return at(topLeft, sprite) || at(topRight, sprite) || at(botRight, sprite) || at(botLeft, sprite);
	}

	/**
    * Returns true if sprite is in the SceneView top region otherwise false.
    * @param sprite the sprite to check (immutable reference)
    * @return true if sprite is in the top region otherwise false.
    */
	bool inTopRegion(const Sprite &sprite)
	{
		int x = sprite.getPosition().x;
		int y = sprite.getPosition().y;
		return x >= 0 && x <= width && y == 0;
	}
	/**
    * Returns true if sprite is in the SceneView right region otherwise false.
    * @param sprite the sprite to check (immutable reference)
    * @return true if sprite is in the right region otherwise false.
    */
	bool inRightRegion(const Sprite &sprite)
	{
		int x = sprite.getPosition().x;
		int y = sprite.getPosition().y;
		return y >= 0 && y <= height && x == width;
	}

	/**
  	* Returns true if sprite is in the SceneView bottom region otherwise false.
  	* @param sprite the sprite to check (immutable reference)
  	* @return true if sprite is in the bottom region otherwise false.
  	*/
	bool inBotRegion(const Sprite &sprite)
	{
		int x = sprite.getPosition().x;
		int y = sprite.getPosition().y;
		return x >= 0 && x <= width && y == height;
	}

	/**
  	* Returns true if sprite is in the SceneView left region otherwise false.
  	* @param sprite the sprite to check (immutable reference)
  	* @return true if sprite is in the left region otherwise false.
  	*/
	bool inLeftRegion(const Sprite &sprite)
	{
		int x = sprite.getPosition().x;
		int y = sprite.getPosition().y;
		return y >= 0 && y <= height && x == 0;
	}
};

/**
  * Moves the referenced sprite via mutation according to its SceneView region and trajectory.
  * @param sprite the sprite to move (mutable)
  * @param sceneView view to check regions of (immutable)
  */
void move(Sprite &sprite, SceneView &sceneView)
{
	if (sceneView.inTopRegion(sprite) && !sceneView.at(sceneView.getTopRight(), sprite) && sprite.getRotation() == 0)
	{
		sprite.move(SPEED_X, 0);
	}
	if (sceneView.inRightRegion(sprite) && !sceneView.at(sceneView.getBotRight(), sprite) && sprite.getRotation() == 90)
	{
		sprite.move(0, SPEED_Y);
	}
	if (sceneView.inBotRegion(sprite) && !sceneView.at(sceneView.getBotLeft(), sprite) && sprite.getRotation() == 180)
	{
		sprite.move(-SPEED_X, 0);
	}
	if (sceneView.inLeftRegion(sprite) && !sceneView.at(sceneView.getTopLeft(), sprite) && sprite.getRotation() == 270)
	{
		sprite.move(0, -SPEED_Y);
	}
}

/**
  * Rotates the referenced sprite via mutation respective to corner point it has reached.
  * @param sprite the sprite to move (mutable)
  * @param sceneView view to check regions of (immutable)
  */
void rotate(Sprite &sprite, SceneView &sceneView)
{
	if (sceneView.at(sceneView.getTopLeft(), sprite))
	{
		sprite.setRotation(0);
	}
	if (sceneView.at(sceneView.getTopRight(), sprite))
	{
		sprite.setRotation(90);
	}
	if (sceneView.at(sceneView.getBotRight(), sprite))
	{
		sprite.setRotation(180);
	}
	if (sceneView.at(sceneView.getBotLeft(), sprite))
	{
		sprite.setRotation(270);
	}
}

/** Represents status of sprite for assisting in when to render a sprite. */
enum SpriteStatus
{
	waiting,
	running,
	justFinished,
	finished
};

/**
  * Runs the program.
  * @return 0 if exit success otherwise failure.
  */
int main()
{

	// Make sprites with boid texture.
	Texture texture;
	texture.loadFromFile("Assets/boid.png");
	vector<Sprite> sprites(NUM_SPRITES, Sprite(texture));
	for (auto &sprite : sprites)
	{
		sprite.setScale(0.05, 0.05);
	}

	// Account for sprites finishing.
	vector<SpriteStatus> spriteStatuses(NUM_SPRITES, SpriteStatus::waiting);
	spriteStatuses[0] = SpriteStatus::running;

	// Setup SceneView.
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);

	// Render in window.
	while (sceneView.scene.isOpen())
	{
		// Handle window event.
		Event event;
		while (sceneView.scene.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				sceneView.scene.close();
				break;
			}
		}

		// Rotate and move sprites accordingly.
		int i = 0;
		int j = 0;
		for (auto &sprite : sprites)
		{

			// Only transform running sprites.
			if (spriteStatuses[i] == SpriteStatus::running)
			{

				// First sprite hit a corner, except for when it is starting.
				if (sceneView.atCorner(sprite) && !sceneView.at(sceneView.getTopLeft(), sprite))
				{
					if (j + 1 <= NUM_SPRITES)
					{
						spriteStatuses[j + 1] = SpriteStatus::running;
					}
				}

				// Sprite has completed a loop.
				if (sceneView.at(sceneView.getTopLeft(), sprite) && sprite.getRotation() == 270)
				{
					spriteStatuses[i] = SpriteStatus::justFinished;
					sprite.setRotation(0);
				}

				// Sprite has not completed a loop.
				else
				{
					rotate(sprite, sceneView);
					move(sprite, sceneView);
				}
			}

			// Increment counters.
			i++;
			j++;
		}

		// If all sprites are finished, reset visualization pattern.
		int numFinished = 0;
		for (auto status : spriteStatuses)
		{
			if (status == SpriteStatus::finished)
			{
				numFinished += 1;
			}
		}
		if (numFinished == NUM_SPRITES)
		{
			for (auto status : spriteStatuses)
			{
				status = SpriteStatus::waiting;
			}
			spriteStatuses[0] = SpriteStatus::running;
		}

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		int k = 0;
		for (auto &sprite : sprites)
		{
			// Finish the justFinished sprite.
			if (spriteStatuses[k] == SpriteStatus::justFinished)
			{
				spriteStatuses[k] = SpriteStatus::finished;
				sceneView.scene.draw(sprite);
			}

			// Only render running sprites.
			if (spriteStatuses[k] == SpriteStatus::running)
			{
				sceneView.scene.draw(sprite);
			}

			// Increment.
			k++;
		}
		sceneView.scene.display();
	}

	// Exit progam.
	return EXIT_SUCCESS;
}
