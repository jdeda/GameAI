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

/** Runs the program.*/
int main(int argc, char* argv[]) {

	// Exit progam.
	return EXIT_SUCCESS;
}
