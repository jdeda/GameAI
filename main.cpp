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
#include <tuple>

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

	float getOrientation() const
	{
		return sprite.getRotation();
	}

	/** Returns the character's id. */
	int getID() const
	{
		return id.getID();
	}

	/** Returns the character's kinematic. */
	Kinematic getKinematic() {
		return kinematic;
	}

	/** Sets the character's kinematic.*/
	void setKinematic(Kinematic kinematic) {
		this->kinematic = kinematic;
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

/** Returns the distance between the two vectors. */
float distance(Vector2f p1, Vector2f p2) {
	return sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
}

/** Returns the distance between the two vectors. */
float distance(Character c1, Character c2) {
	return sqrt(pow(c1.y() - c1.y(), 2) + pow(c2.x() - c1.x(), 2));
}

class OrientationTable
{

private:
	unordered_map<int, float> table;

public:
	OrientationTable(const unordered_map<int, float>& orientations)
	{
		table = orientations;
	}

	float getOldOrientation(const Character &character) const
	{
		return table.at(character.getID());
	}
};

class PositionTable
{

private:
	unordered_map<int, Vector2f> table;

public:
	PositionTable(const unordered_map<int, Vector2f>& positions)
	{
		table = positions;
	}

	Vector2f getOldPosition(const Character &character) const
	{
		return table.at(character.getID());
	}

	void debug(const Character &character) const
	{
		Vector2f p = table.at(character.getID());
		cout << character.getID() << " " << p.x << " " << p.y << endl;
	}
};

class CharacterTable
{
private:
	unordered_map<int, Character*> table;
	vector<Character*> characters;

public:
	CharacterTable(const vector<Character*>& characters)
	{
		for(auto & character: characters) {
			this->table.insert({character->getID(), character});
			this->characters.push_back(character);
		}
	}

	inline PositionTable generatePositionTable() {
		unordered_map<int, Vector2f> positions;
		for(auto & character: characters) {
			positions.insert({character->getID(), character->getPosition()});
		}
		return PositionTable(positions);
	}

	inline OrientationTable generateOrientationTable() {
		unordered_map<int, float> orientations;
		for(auto & character: characters) {
			orientations.insert({character->getID(), character->getOrientation()});
		}
		return OrientationTable(orientations);
	}

	inline void updateKinematics(float dt, const PositionTable& positions, const OrientationTable& orientations) {
		for(auto & character: characters) {
			character->setKinematic(computeKinematic(
				dt, positions.getOldPosition(*character), character->getPosition(),
				orientations.getOldOrientation(*character), character->getOrientation()
			));
		}
	}
};

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
	Vector2f mousePositionOld(mouse.getPosition());

	// Setup SceneView.
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);

	// Setup CharacterTable and PositionTable.
	vector<Character*> characters;
	characters.push_back(&character);
	CharacterTable characterTable(characters);
	PositionTable positionTable = characterTable.generatePositionTable();
	OrientationTable orientationTable = characterTable.generateOrientationTable();

	// Render scene and measure time.
	Clock clock;
	while (sceneView.scene.isOpen())
	{
		// Delta time. Handle real-time time, not framing based time. Simply print dt to console and see it work.
		float dt = clock.restart().asSeconds();
		// positionTable.debug(character);

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

		// Generate kinematics for every character.
		Vector2f mousePositionNew(mouse.getPosition(sceneView.scene));
		mouseKinematic = computeKinematic(dt, mousePositionOld, mousePositionNew, 0, 0);
		characterTable.updateKinematics(dt, positionTable, orientationTable);

		// Velocity match character to the mouse.
		SteeringOutput match = velocityMatcher.calculateAcceleration(character.getKinematic(), mouseKinematic);
		character.update(match, dt);

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		sceneView.scene.draw(character.sprite);
		sceneView.scene.display();

		// Update positions and orientations of previous loop.
		positionTable = characterTable.generatePositionTable();
		orientationTable = characterTable.generateOrientationTable();
		mousePositionOld = Vector2f(mouse.getPosition());
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
