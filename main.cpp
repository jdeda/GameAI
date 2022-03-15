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
#include "search/search.h"
#include "search/dijsktra.h"

using namespace std;
using namespace sf;

const float MAZE_X = 10.f;
const float MAZE_Y = 10.f;

/** Start unique IDs at 0. */
int ID::count = 0;

/** Size of each tile in the Maze. */
const float SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
Vector2f LevelCell::dims = Vector2f(SIZE, SIZE);

/** Animates the velocity match steering behavior. */
void SmallGraphVisualizer(Algorithm algorithm) {

	// Find path through maze.
	Maze maze(MAZE_X, MAZE_Y);
	Level level = maze.getLevel();
	Graph graph = maze.getGraph();
	// for (int i = 0; i < graph.getRows(); i++) {
	// 	for (int j = 0; j < graph.getCols(); j++) {
	// 		if (i == 0 || i == MAZE_X - 1 || j == 0 | j == MAZE_Y - 1) { continue; }
	// 		if (!level.cells[i][j].inLevel) { continue; }
	// 		GraphNode current = graph.quantize(Location(i, j));
	// 		vector<Edge> edges = current.getEdges();
	// 		Location a = graph.quantize(current.getLocation()).getLocation();
	// 		cout << "location: " << a.x << " " << a.y << endl;
	// 		cout << "neighbors:" << endl;
	// 		for (const auto& edge : edges) {
	// 			Location b = graph.localize(edge.getToVertex());
	// 			cout << b.x << " " << b.y << endl;
	// 		}
	// 		cout << "directions:" << endl;
	// 		int count = 0;
	// 		for (auto dir : level.cells[i][j].directions) {
	// 			if (dir) {
	// 				cout << count << " ";
	// 				if (count == 0) {
	// 					cout << "right" << " ";
	// 				}
	// 				if (count == 1) {
	// 					cout << "up" << " ";
	// 				}
	// 				if (count == 2) {
	// 					cout << "down" << " ";
	// 				}
	// 				if (count == 3) {
	// 					cout << "left" << " ";
	// 				}
	// 				cout << endl;
	// 			}
	// 			count += 1;
	// 		}
	// 		cout << endl << endl;
	// 	}
	// }


	// for (int i = 0; i < graph.getRows(); i++) {
	// 	for (int j = 0; j < graph.getCols(); j++) {
	// 		if(i == 0 || i == MAZE_X - 1 || j == 0 | j == MAZE_Y - 1) { continue; }
	// 		if(!level.cells[i][j].inLevel) { continue; }
	// 		cout <<  "cell: " << i << " " << j << endl;
	// 		auto dirs = level.cells[i][j].directions;
	// 		// i said right,up,down,left
	// 		// originally it was: (left, up, down, right)
	// 		for(int i = 0; i < 4; i++) {
	// 				cout << dirs[i] << " ";
	// 		}
	// 		cout << endl << endl;
	// 	}
	// }

    // graph: left right down up
    // me: right up down left
	for (int i = 0; i < graph.getRows(); i++) {
		for (int j = 0; j < graph.getCols(); j++) {
			if (i == 0 || i == MAZE_X - 1 || j == 0 | j == MAZE_Y - 1) { continue; }
			if (!level.cells[i][j].inLevel) { continue; }
			cout << "cell: " << i << " " << j << endl;
			auto dirs = level.cells[i][j].directions;
			// i said right,up,down,left
			// originally it was: (left, up, down, right)
			for (int i = 0; i < 4; i++) {
				bool dir = dirs[i];
				if (dir) {
					        // TRANSPOSE!: this is really: down, right, left, up

					if (i == 0) {
						cout << "down" << " ";
					}
					if (i == 1) {
						cout << "right" << " ";
					}
					if (i == 2) {
						cout << "left" << " ";
					}
					if (i == 3) {
						cout << "up" << " ";
					}
					cout << endl;
				}
			}
			cout << endl << endl;
		}
	}


// Dijkstra search = Dijkstra(maze.getGraph(), Location(1, 1), Location(3, 9));
// Path path = search.search();

// Render the path in the maze.
// SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);
// while (sceneView.scene.isOpen()) {

// 	// Handle scene poll event.
// 	Event event;
// 	while (sceneView.scene.pollEvent(event)) {
// 		switch (event.type) {
// 			case Event::Closed:
// 				sceneView.scene.close();
// 				break;
// 		}
// 	}

// 	// Re-render scene.
// 	sceneView.scene.clear(Color(255, 255, 255));
// 	maze.draw(&sceneView.scene);
// 	sceneView.scene.display();
// }
}

// void BigGraphVisualizer(Algorithm algorithm) {}
// void CharacterGraphVisualizer(Algorithm algorithm) {}

/** Runs the program.*/
int main(int argc, char* argv[]) {

	srand(1);

	// greeting();
	// Visualizer visualizer = getVisualizer();
	// Algorithm algorithm = getAlgorithm();

	// switch (visualizer) {
	// 	case Visualizer::small:
	// 		SmallGraphVisualizer(algorithm);
	// 		break;
	// 	case Visualizer::big:
	// 		BigGraphVisualizer(algorithm);
	// 		break;
	// 	case Visualizer::character:
	// 		CharacterGraphVisualizer(algorithm);
	// 		break;
	// 	case Visualizer::INVALID_VIS:
	// 		fail("invalid visualizer choice");
	// 		break;

	// }

	SmallGraphVisualizer(Algorithm::DIJKSTRA);

	return EXIT_SUCCESS;
}