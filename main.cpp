#include <cmath>
#include "debug/debug.h"
#include "userinput/userinput.h"
#include "hparams/hyperparameters.h"
#include "id/id.h"
#include "level/level.h"
#include "visualizers/visualizers.h"

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