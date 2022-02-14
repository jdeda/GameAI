/**
 * Program displays a window animating different steering behaviors.
 * @file main.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "steering.cpp"

using namespace sf;
using namespace std;

/** Number of arguments for program. */
const int ARGC = 2;

/* SceneView X length. */
const int SCENE_WINDOW_X = 640;

/* SceneView Y length. */
const int SCENE_WINDOW_Y = 480;

/* SceneView frame rate. */
const int SCENE_WINDOW_FR = 100;

/* Expected speed for sprite moving in X dimensions. */
const float SPEED_X = 10.f;

/* Expected speed for sprite moving in Y dimensions. */
const float SPEED_Y = (float(SCENE_WINDOW_Y) / float(SCENE_WINDOW_X)) * float(SPEED_X);

/* Number of sprites. */
const int NUM_CHARACTERS = 4;

/** Represents a unique ID. */
class ID {
	private:
		static int count;
		int id;
	public:

	 	/** Sets the id of the instance and increments the static count (pattern to preserve uniqueness). */
		ID()
		{
			id = count + 1;
			count += 1;
		}

		int getID() const
		{
			return id;
		}
};
int ID::count = 0;

/** Exits program and displays message to console*/
void fail(string message) {
	cout << "error: " << message << endl;
	exit(1);
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

private:

	/** Character's kinematic data. */
	Kinematic kinematic;

	/** Character's unique ID. */
	ID id;

public:
	/** Constructs a Character with a unique ID. */
	Character() {
		id = ID();
	}

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

	Vector2f getPosition() const
	{
		return sprite.getPosition();
	}

	/** Returns the character's id. */
	int getID() const
	{
		return id.getID();
	}

	/** Returns the charcter's kinematic. */
	Kinematic getKinematic() {
		return kinematic;
	}

   /**
	* Updates character's sprite and kinematic.
     * @param steering the steering output to apply (immutable)
     * @param time the change in time since last update (copy)
	*/
	void update(const SteeringOutput& steering, float time) {
		kinematic.update(steering, time);
		// Notice, the sprite should only accessible here, for now,
		// we will leave it public for simplicity.

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

/** Returns the distance between the two vectors. */
float distance(Vector2i p1, Vector2i p2) {
	return sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
}

/** Returns the distance between the two vectors. */
float distance(Character c1, Character c2) {
	return sqrt(pow(c1.y() - c1.y(), 2) + pow(c2.x() - c1.x(), 2));
}


class PositionTable
{

private:
	vector<Character> table;

public:
	PositionTable(const vector<Character>& characters)
	{
		table = vector<Character>(characters); // TODO: Does this copy the objects...?
	}

	Vector2f getOldPosition(const Character &character) const
	{
		for(const Character &c: table) {
			if(c.getID() == character.getID()) {
				return c.getPosition();
			}
		}
	}

	float getDelta(const Character &character) const
	{
		for(const Character &c: table) {
			if(c.getID() == character.getID()) {
				return distance(c, character);
			}
		}
	}

	void update(const vector<Character>& characters)
	{
		table = vector<Character>(characters);
	}

	void debug() const
	{
		for(const Character& character: this->table)
		{
			cout << character.x() << " " << character.y() << endl;
		}
	}

};


/** Animates the arrive steering behavior. */
void ArriveAnimation() {

	// Setup Arrive algorithm.
	Velocity velocityMatcher(0.01);

	// Setup character.
	float scale = 0.05;
	Texture texture;
	texture.loadFromFile("assets/boid.png");
	Character character;
	character.scale = scale;
	character.texture = texture;
	character.sprite = *(new Sprite(texture));
	character.sprite.setScale(scale, scale);
	character.status = CharacterStatus::running;

	// Setup mouse.
	Mouse mouse;
	Kinematic mouseKinematic;
	Vector2i mousePositionOld = mouse.getPosition();

	// Setup SceneView.
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);

	// Setup PositionTable.
	vector<Character> characters;
	characters.push_back(character);
	PositionTable positionTable(characters);

	// Render scene and measure time.
	Clock clock;
	while (sceneView.scene.isOpen())
	{
		positionTable.debug();

		// How does this dt work? Calling clock.restart().asSecondss()
		// Delta time. Handle real-time time, not framing based time. Simply print dt to console and see it work.
		float dt = clock.restart().asSeconds();
		Vector2i mousePosition1 = mouse.getPosition(sceneView.scene);

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

	// // Set character's position to that of the mouse.
	// Vector2i mousePosition = mouse.getPosition(sceneView.scene);
	// cout << mousePosition.x << " " << mousePosition.y << endl;
	// character.sprite.setPosition(mousePosition.x, mousePosition.y);

	// Velocity match character to mouse.
	// need kinematic of both to compute acceeleration
	// need time for the update.. so...
	// problem is...what is the mouse kinematic?
	// sfml only gives us position, anything with time we have to
	// figure out ourselves.
	// so...
	Vector2i mousePosition2 = mouse.getPosition(sceneView.scene);
	float mouseDistanceDelta = distance(mousePosition2, mousePosition1);
	


	// mouseKinematic.position = mouse.getPosition(); // v2f
	// mouseKinematic.orientation = 0; // not needed?
	// mouseKinematic.velocity = 0; // v2f..
	// mouseKinematic.rotation = 0; // not needed?



	
	// Make character move at same speed as mouse.
	// // Update mouse kinematic.
	// mouseKinematic.position = mouse.getPosition(); // v2f
	// mouseKinematic.orientation = 0; // not needed?
	// mouseKinematic.velocity = 0; // v2f
	// mouseKinematic.rotation = 0; // not needed?
	// // Velocity match character to mouse.
	// float deltaTime = getDeltaTime(elapsedTimeFromPreviousLoopIteration, clock.getElapsedTime());
	// cout << deltaTime << endl;
	// SteeringOutput steering = velocityMatcher.calculateAcceleration(character.getKinematic(), mouseKinematic);
	// character.update(steering, deltaTime);
	
	// Re-render scene.
	sceneView.scene.clear(Color(255, 255, 255));
	sceneView.scene.draw(character.sprite);
	sceneView.scene.display();

	// Update position table.
	positionTable.update(characters);
	mousePositionOld = mouse.getPosition();
	}
}

/** Animates the wander steering behavior. */
void WanderAnimation() {

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

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		sceneView.scene.display();
	}
}

/** Animates the flocking steering behavior. */
void FlockAnimation() {

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

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		sceneView.scene.display();
	}
}

/** Represents possible steering behavior algorithms for switching over and running animations. */
enum Algorithm {
	ArriveAlign,
	Wander,
	Flock,
	INVALID
};

/** Algorithms represented as strings. */
vector<string> AlgorithmStrings = {
	"ArriveAlign",
	"Wander",
	"Flock",
	"INVALID"
};

/** Returns an Algorithm if input is valid, otherwise fail program. */
Algorithm getAlg() {

	// Display user menu.
	cout << "**************************************" << endl;
	cout << "*****Welcome to SteeringBehaviors*****" << endl;
	cout << "**************************************" << endl;
	cout << "Choose an algorithm to visualize" << endl;
	for(int i = 0; i < AlgorithmStrings.size() - 1; i++) {
		cout << i + 1 << ". " << AlgorithmStrings[i] << endl;
	}

	// Get algorithm number.
	int caseNum = 0;
	cout << "Enter an algorithm number: ";
	cin >> caseNum; 
	if (cin.bad()) { fail("invalid algorithm choice"); }

	// Map to enum.
	caseNum--;
	if (caseNum < 0 || caseNum > AlgorithmStrings.size()) { return Algorithm::INVALID; }
	else { return Algorithm(caseNum); }
}

/* Debug output (prints the sprites coordinates). */
void debug(const Character &character)
{
	cout << character.x() << " " << character.y() << endl;
}

/* Debug output (for sanity check). */
void sanity()
{
	cout << "sanity" << endl;
}

/** Runs the program.*/
int main(int argc, char *argv[])
{
	// Get algorithm and run corresponding algorithm.
	Algorithm alg = getAlg();
	switch (alg) {
		case Algorithm::ArriveAlign:
			ArriveAnimation();
			break;
		case Algorithm::Wander:
			WanderAnimation();
			break;
		case Algorithm::Flock:
			FlockAnimation();
			break;
		case Algorithm::INVALID:
			fail("invalid algorithm choice");
			break;
	}

	// Exit progam.
	return EXIT_SUCCESS;
}
