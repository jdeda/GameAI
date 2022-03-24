/**
 * Program displays a window animating different steering behaviors.
 * @file main.cpp
 * @author Jese Deda
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <iostream>
#include "debug/debug.h"
#include "debug/breadcrumbs.h"
#include "userinput/userinput.h"
#include "id/id.h"
#include "scene/scene.h"
#include "tables/tables.h"
#include "character/character.h"
#include "kinematic/kinematic.h"
#include "hparams/hyperparameters.h"
#include "steering/steering.h"
#include "steering/steeringoutput.h"
#include "graph/graph.h"
#include "level/level.h"
#include "maze/maze.h"
#include "search/search.h"
#include "search/a*.h"
#include "decision/decisiontree.h"
#include "decision/behaviortree.h"
#include "decision/decisiontreelearning.h"

using namespace std;
using namespace chrono;
using namespace sf;

/** Start unique IDs at 0. */
int ID::count = 0;

/** Size of each tile in the Maze. */
float MAZE_X = 0.f;
float MAZE_Y = 0.f;
float SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
Vector2f LevelCell::dims = Vector2f(SIZE, SIZE);

const bool BREAD_CRUMBS = true;

int main(int argc, char* argv[]) {
	srand(1);
	auto visualizer = getVisualizer();
	if (visualizer == NULL) { fail("invalid visualizer choice"); }
	switch (*visualizer) {
		case Visualizer::DecisionTree:
			break;
		case Visualizer::BehaviorTree:
			break;
		case Visualizer::DecisionTreeLearning:
			break;
	};
	return EXIT_SUCCESS;
}