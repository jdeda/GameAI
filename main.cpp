#include "hparams/hyperparameters.h"
#include "id/id.h"
#include "level/level.h"
#include "visualizers/visualizer.h"

int ID::count = 0;
Vector2f LevelCell::dims = Vector2f(SIZE, SIZE);

int main() {

	// Run visualizer.
	srand(1);
	Visualizer();
	return EXIT_SUCCESS;
}