/**
 * Program displays a window animating different steering behaviors.
 * @file main.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "steering.cpp"

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
const int NUM_CHARACTERS = 4;

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

/** Represents status of character for assisting in when to render a character. */
enum CharacterStatus
{
	waiting,
	running,
	justFinished,
	finished
};

/** Character is a sprite with additional attributes and behaviors. */
class Character
{

public:
	/** Constructs a Character. */
	Character() {}

	/** Character's texture. */
	Texture texture;

	/** Character's sprite. */
	Sprite sprite;

	/** Character's scale (how large or small character is). */
	float scale;

	/** Character's status (used for rendering purposes). */
	CharacterStatus status;

	/** Returns the character's x position. */
	int x() const
	{
		return sprite.getPosition().x;
	}

	/** Returns the character's y position. */
	int y() const
	{
		return sprite.getPosition().y;
	}
};

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
 	* Returns true if character is at a point otherwise false.
  	* @param point the point to compare sprite position to (immutable reference)
  	* @param character the character to check (immutable reference)
  	* @return true if character is at a point otherwise false.
  	*/
	bool at(const Vector2f &point, const Character &character)
	{
		return character.x() == point.x && character.y() == point.y;
	}

	bool atCorner(const Character &character)
	{
		return at(topLeft, character) || at(topRight, character) || at(botRight, character) || at(botLeft, character);
	}

	/**
    * Returns true if character is in the SceneView top region otherwise false.
    * @param character the character to check (immutable reference)
    * @return true if character is in the top region otherwise false.
    */
	bool inTopRegion(const Character &character)
	{
		int x = character.x();
		int y = character.y();
		return x >= 0 && x <= width && y == 0;
	}
	/**
    * Returns true if character is in the SceneView right region otherwise false.
    * @param character the character to check (immutable reference)
    * @return true if character is in the right region otherwise false.
    */
	bool inRightRegion(const Character &character)
	{
		int x = character.x();
		int y = character.y();
		return y >= 0 && y <= height && x == width;
	}

	/**
  	* Returns true if character is in the SceneView bottom region otherwise false.
  	* @param character the character to check (immutable reference)
  	* @return true if character is in the bottom region otherwise false.
  	*/
	bool inBotRegion(const Character &character)
	{
		int x = character.x();
		int y = character.y();
		return x >= 0 && x <= width && y == height;
	}

	/**
  	* Returns true if character is in the SceneView left region otherwise false.
  	* @param character the character to check (immutable reference)
  	* @return true if character is in the left region otherwise false.
  	*/
	bool inLeftRegion(const Character &character)
	{
		int x = character.x();
		int y = character.y();
		return y >= 0 && y <= height && x == 0;
	}
};

/**
  * Moves the referenced character via mutation according to its SceneView region and trajectory.
  * @param character the character to move (mutable)
  * @param sceneView view to check regions of (immutable)
  */
void move(Character &character, SceneView &sceneView)
{
	// Move right.
	if (sceneView.inTopRegion(character) && !sceneView.at(sceneView.getTopRight(), character) && character.sprite.getRotation() == 0)
	{
		character.sprite.move(SPEED_X, 0);
	}

	// Move down.
	if (sceneView.inRightRegion(character) && !sceneView.at(sceneView.getBotRight(), character) && character.sprite.getRotation() == 90)
	{
		character.sprite.move(0, SPEED_Y);
	}

	// Move left.
	if (sceneView.inBotRegion(character) && !sceneView.at(sceneView.getBotLeft(), character) && character.sprite.getRotation() == 180)
	{
		character.sprite.move(-SPEED_X, 0);
	}

	// Move up.
	if (sceneView.inLeftRegion(character) && !sceneView.at(sceneView.getTopLeft(), character) && character.sprite.getRotation() == 270)
	{
		character.sprite.move(0, -SPEED_Y);
	}
}

/**
  * Rotates the referenced character via mutation respective to corner point it has reached.
  * @param character the character to move (mutable)
  * @param sceneView view to check regions of (immutable)
  */
void rotate(Character &character, SceneView &sceneView)
{

	// Don't rotate (just started).
	if (sceneView.at(sceneView.getTopLeft(), character))
	{
		character.sprite.setRotation(0);
	}

	// Face down (time to move down).
	if (sceneView.at(sceneView.getTopRight(), character))
	{
		character.sprite.setRotation(90);
	}

	// Face left (time to move left).
	if (sceneView.at(sceneView.getBotRight(), character))
	{
		character.sprite.setRotation(180);
	}

	// Face up (time to move up).
	if (sceneView.at(sceneView.getBotLeft(), character))
	{
		character.sprite.setRotation(270);
	}
}

/**
  * Runs the program.
  * @return 0 if exit success otherwise failure.
  */
int main()
{

	// Make characters;
	Texture texture;
	texture.loadFromFile("Assets/boid.png");
	float scale = 0.05;
	vector<Character> characters(NUM_CHARACTERS, Character());
	for (auto &character : characters)
	{
		character.scale = scale;
		character.texture = texture;
		character.sprite = *(new Sprite(texture));
		character.sprite.setScale(scale, scale);
	}
	characters[0].status = CharacterStatus::running; // Only first character runs at first.

	// Setup SceneView.
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);

	// Render scene and measure time.
	Clock clock;
	while (sceneView.scene.isOpen())
	{
		// Handle scene poll event.
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

		// Rotate and move characters accordingly.
		int i = 0;
		int j = 0;
		for (auto &character : characters)
		{
			// Only transform running characters.
			if (character.status == CharacterStatus::running)
			{

				// First character hit a corner, except for when it is starting.
				if (sceneView.atCorner(character) && !sceneView.at(sceneView.getTopLeft(), character))
				{
					if (j + 1 <= NUM_CHARACTERS)
					{
						characters[j + 1].status = CharacterStatus::running;
					}
				}

				// Character has completed a loop.
				if (sceneView.at(sceneView.getTopLeft(), character) && character.sprite.getRotation() == 270)
				{
					character.status = CharacterStatus::justFinished;
					character.sprite.setRotation(0);
				}

				// Character has not completed a loop.
				else
				{
					rotate(character, sceneView);
					move(character, sceneView);
				}
			}

			// Increment counters.
			i++;
			j++;
		}

		// If all characters are finished, reset visualization pattern.
		int numFinished = 0;
		for (auto &character : characters)
		{
			if (character.status == CharacterStatus::finished)
			{
				numFinished += 1;
			}
		}
		if (numFinished == NUM_CHARACTERS)
		{
			for (auto &character : characters)
			{
				character.status = CharacterStatus::waiting;
			}
			characters[0].status = CharacterStatus::running;
		}

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		for (auto &character : characters)
		{
			// Finish the justFinished character.
			if (character.status == CharacterStatus::justFinished)
			{
				character.status = CharacterStatus::finished;
				sceneView.scene.draw(character.sprite);
			}

			// Only render running characters.
			if (character.status == CharacterStatus::running)
			{
				sceneView.scene.draw(character.sprite);
			}
		}
		sceneView.scene.display();
	}

	// Exit progam.
	return EXIT_SUCCESS;
}
