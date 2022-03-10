/**
 * Program displays a window animating different steering behaviors.
 * @file main.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include "debug/debug.h"
#include "debug/breadcrumbs.h"
#include "userinput/userinput.h"
#include "id/id.h"
#include "scene/scene.h"
#include "tables/tables.h"
#include "character/character.h"
#include "kinematic/kinematic.h"
#include "steering/steering.h"
#include "steering/steeringoutput.h"
#include "graph/graph.h"
#include "level/level.h"
#include "maze/maze.h"

using namespace std;
using namespace sf;

const float MAZE_X = 20.f;
const float MAZE_Y = 20.f;

/** Start unique IDs at 0. */
int ID::count = 0;

const float SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
Vector2f LevelCell::dims = Vector2f(SIZE, SIZE);

/** Animates the velocity match steering behavior. */
void SmallGraphVisualizer(Algorithm algorithm) {
	Maze maze = Maze(MAZE_X, MAZE_Y);
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);
	Clock clock;
	while (sceneView.scene.isOpen()) {

		// Handle scene poll event.
		Event event;
		while (sceneView.scene.pollEvent(event)) {
			switch (event.type) {
				case Event::Closed:
					sceneView.scene.close();
					break;
			}
		}

		// Re-render scene.
		sceneView.scene.clear(Color(255, 255, 255));
		maze.draw(&sceneView.scene);
		sceneView.scene.display();
	}
}

void BigGraphVisualizer(Algorithm algorithm) {}
void CharacterGraphVisualizer(Algorithm algorithm) {}


/** Runs the program.*/
int main(int argc, char* argv[]) {

	greeting();
	Visualizer visualizer = getVisualizer();
	Algorithm algorithm = getAlgorithm();

	switch (visualizer) {
		case Visualizer::small:
			SmallGraphVisualizer(algorithm);
			break;
		case Visualizer::big:
			BigGraphVisualizer(algorithm);
			break;
		case Visualizer::character:
			CharacterGraphVisualizer(algorithm);
			break;
		case Visualizer::INVALID_VIS:
			fail("invalid visualizer choice");
			break;

	}

	// SmallGraphVisualizer(Algorithm::DIJKSTRA);

	return EXIT_SUCCESS;
}