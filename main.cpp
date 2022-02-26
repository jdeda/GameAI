/**
 * Program displays a window animating different steering behaviors.
 * @file main.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include "debug/debug.h"
#include "debug/breadcrumbs.h"
#include "id/id.h"
#include "scene/scene.h"
#include "tables/tables.h"
#include "character/character.h"
#include "kinematic/kinematic.h"
#include "steering/steering.h"
#include "steering/steeringoutput.h"
#include "graph/graph.h"
#include "maze/maze.h"

using namespace sf;
using namespace std;

/** Start unique IDs at 0. */
int ID::count = 0;

/** Animates the velocity match steering behavior. */
void SmallGraphVisualizer() {

	// Generate graph.
	Level maze = generateMaze(10, 10);
	maze.print();

	// Setup SceneView.
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);

	// // Render scene and measure time.
	// Clock clock;
	// while (sceneView.scene.isOpen())
	// {
	// 	// Delta time. Handle real-time time, not framing based time. Simply print dt to console and see it work.
	// 	float dt = clock.restart().asSeconds();

	// 	// Handle scene poll event.
	// 	Event event;
	// 	while (sceneView.scene.pollEvent(event))
	// 	{
	// 		switch (event.type)
	// 		{
	// 		case Event::Closed:
	// 			sceneView.scene.close();
	// 			break;
	// 		}
	// 	}


	// 	// Re-render scene.
	// 	sceneView.scene.clear(Color(255, 255, 255));
	// 	sceneView.scene.draw(maze.toSFML());
	// 	sceneView.scene.display();
	// }
}


/** Runs the program.*/
int main(int argc, char* argv[]) {

	SmallGraphVisualizer();

	// Exit progam.
	return EXIT_SUCCESS;
}
