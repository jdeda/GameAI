#include <cmath>
#include "debug/debug.h"
#include "userinput/userinput.h"
#include "hparams/hyperparameters.h"
#include "id/id.h"
#include "level/level.h"
#include "visualizers/visualizers.h"

/** Start unique IDs at 0. */
int ID::count = 0;

/** Size of each tile in the Maze. */
float MAZE_X = 0.f;
float MAZE_Y = 0.f;
float SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
Vector2f LevelCell::dims = Vector2f(SIZE, SIZE);

const bool BREAD_CRUMBS = false;

int main() {

	// Run visualizer.
	srand(1);
	auto visualizer = getVisualizer();
	if (visualizer == NULL) { fail("invalid visualizer choice"); }
	switch (*visualizer) {
		case Visualizer::DecisionTree:
			DecisionTreeVisualizer();
		case Visualizer::BehaviorTree:
			BehaviorTreeVisualizer();
		case Visualizer::DecisionTreeLearning:
			DecisionTreeLearningVisualizer();
	};

	// Free memory and exit.
	delete visualizer;
	return EXIT_SUCCESS;
}