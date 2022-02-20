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
#include "hyperparameters.h"
#include <cmath>

using namespace sf;
using namespace std;

/* Debug output (prints the vector coordinates). */
void debug(Vector2f v)
{
	cout << v.x << " " << v.y << endl;
}

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
     * @param time the change in time since last update (immutable)
	 * @param clip if true clip otherwise don't (immutable)
	*/
	void update(const SteeringOutput& steering, const float dt, const bool clip) {
		kinematic.update(steering, dt, clip);
		sprite.setPosition(kinematic.position);
		sprite.setRotation(kinematic.orientation);
		// debug(kinematic.linearVelocity);
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

		void debug(const Character &character) const
		{
			float o = table.at(character.getID());
			cout << character.getID() << " " << o << endl;
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

/** Animates the velocity match steering behavior. */
void VelocityMatchAnimation() {

	// Setup velocity matcher.
	Velocity velocityMatcher(TIME_TO_REACH_TARGET_VELOCITY);

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
	bool clip = false;

	// Render scene and measure time.
	Clock clock;
	while (sceneView.scene.isOpen())
	{
		// Delta time. Handle real-time time, not framing based time. Simply print dt to console and see it work.
		float dt = clock.restart().asSeconds();

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

		/**
		 * Update mouse kinematic and velocity match charcter to mouse.
		 *
		 * Notes on this behavior:
		 * Mouse will generate a new kinematic and update itself with an empty steering behavior. The mouse moves automatically and thus its kinematic needs to be computed
		 * (AKA set its velocities) and updated (AKA needs to apply its own movement). The character's however do not need to do any of this. Why? The character's movements
		 * rely soly on the mouse's movements. The character is velocity matching the mouse, so all it needs to do is apply the accelerations computed from the matcher function.
		 * It does not move itself, it moves from the matcher function: it is simply following the provided calculations to velocity match the mouse.
		 */
		Vector2f mousePositionNew(mouse.getPosition(sceneView.scene));
		mouseKinematic = computeKinematic(dt, mousePositionOld, mousePositionNew, 0, 0); // TODO: 0s may need to be computed mathematically
		mouseKinematic.update(SteeringOutput(), dt, clip);
		SteeringOutput match = velocityMatcher.calculateAcceleration(character.getKinematic(), mouseKinematic);
		character.update(match, dt, clip);

		// debug(character);
		// debug(character.getKinematic().position);
		// BIG PROLEM. THESE DO NOT MATCH AT ALL.

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		sceneView.scene.draw(character.sprite);
		sceneView.scene.display();

		// Update positions and orientations of previous loop.
		positionTable = characterTable.generatePositionTable();
		orientationTable = characterTable.generateOrientationTable();
		mousePositionOld = Vector2f(mouse.getPosition(sceneView.scene));
	}
}

/** Amimates the arrive and align steering behavior. */
void ArriveAlignAnimation()  {

	// Setup arrive-align matchers.
	Position positionMatcher(TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
	Orientation orientationMatcher(TIME_TO_REACH_TARGET_ROTATION, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_ROTATION);
	SteeringComposer steeringComposer;

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
	character.sprite.setPosition(SCENE_WINDOW_X / 2, SCENE_WINDOW_Y / 2);

	// Setup click position data.
	Vector2f mouseClickPosition(0.f, 0.f);
	float mouseClickOrientation = 0;

	// Setup SceneView.
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);

	// Setup CharacterTable and PositionTable.
	vector<Character*> characters;
	characters.push_back(&character);
	CharacterTable characterTable(characters);
	PositionTable positionTable = characterTable.generatePositionTable();
	OrientationTable orientationTable = characterTable.generateOrientationTable();
	bool clip = true;

	Vector2f currentCharacterPosition;
	Vector2f distVector;

	// Render scene and measure time.
	Clock clock;
	while (sceneView.scene.isOpen())
	{
		// Delta time. Handle real-time time, not framing based time. Simply print dt to console and see it work.
		float dt = clock.restart().asSeconds();

		// Handle scene poll event.
		Event event;
		while (sceneView.scene.pollEvent(event))
		{
			switch (event.type)
			{

			// Mouse clicked.
			case Event::MouseButtonPressed:
				mouseClickPosition = Vector2f(Mouse::getPosition(sceneView.scene));
				currentCharacterPosition = character.getKinematic().position;
				distVector = mouseClickPosition - currentCharacterPosition;
				mouseClickOrientation = atan2(distVector.y, distVector.x) * (180.f / M_PI) - 45;

				// mouseClickOrientation = atan2(character.getKinematic().position.x,character.getKinematic().position.y) * (180 / M_PI);
				// disp = mouseK.getKinematic().linearVelocity - character.getKinematic().linearVelocity;
				// mouseClickOrientation = atan2(disp.x, disp.y) * (180.f / M_PI);
				cout << mouseClickOrientation << endl;
				// debug(mouseClickPosition);
				break;

			// Close scene.
			case Event::Closed:
				sceneView.scene.close();
				break;
			}
		}

		// Have mouse arrive-align wherever the latest mouse click was.
		Kinematic mouseKinematic;
		mouseKinematic.position = mouseClickPosition;
		mouseKinematic.orientation = mouseClickOrientation;
		// cout << mouseClickOrientation << endl;
		if(mouseClickPosition != Vector2f(0.f, 0.f)) {
			SteeringOutput arrive = positionMatcher.calculateAcceleration(character.getKinematic(), mouseKinematic);
			SteeringOutput align = orientationMatcher.calculateAcceleration(character.getKinematic(), mouseKinematic);
			character.update(arrive, dt, clip);
			character.update(align, dt, clip);
		}

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		sceneView.scene.draw(character.sprite);
		sceneView.scene.display();

		// Update positions and orientations of previous loop.
		positionTable = characterTable.generatePositionTable();
		orientationTable = characterTable.generateOrientationTable();
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
	VelocityMatch,
	ArriveAlign,
	Wander,
	Flock,
	INVALID
};

/** Algorithms represented as strings. */
vector<string> AlgorithmStrings = {
	"VelocityMatch",
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
		case Algorithm::VelocityMatch:
			VelocityMatchAnimation();
			break;
		case Algorithm::ArriveAlign:
			ArriveAlignAnimation();
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
