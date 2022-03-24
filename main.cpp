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
#include "search/dijsktra.h"
#include "search/a*.h"

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

const bool BREAD_CRUMBS = false;

/** Renders the path through the maze.*/
void Visualize(const Maze& maze, const Path& path) {
	cout << "Rendering solution..." << endl;
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);
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
		path.draw(&sceneView.scene);
		sceneView.scene.display();
	}
}

/** Switches over given algorithm for a maze path finding visualization. */
void VisualizeSwitch(Algorithm algorithm, const Maze& maze, const Location& start, const Location& end) {
	cout << "Searching maze..." << endl;
	switch (algorithm) {
		case DIJKSTRA:
			{
				Dijkstra search(maze.getGraph(), start, end);
				Path path = search.search();
				Visualize(maze, path );
				break;
			}
		case A_STAR_H1:
			{
				AStar search(maze.getGraph(), start, end, ManhattanHeuristic(end));
				Path path = search.search();
				Visualize(maze, path);
				break;
			}
		case A_STAR_H2:
			{
				AStar search(maze.getGraph(), start, end, EuclideanHeuristic(end));
				Path path = search.search();
				Visualize(maze, path);
				break;
			}
		case A_STAR_CUSTOM:
			{
				AStar search(maze.getGraph(), start, end, CustomHeuristic(end));
				Path path = search.search();
				Visualize(maze, path);
				break;
			}
		case INVALID_ALG:
			{
				fail("invalid algorithm choice");
				break;
			}
	}
}

/** Renders path through a small maze. */
void SmallGraphVisualizer(Algorithm algorithm) {

	MAZE_X = 20;
	MAZE_Y = 20;
	SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
	LevelCell::dims = Vector2f(SIZE, SIZE);
	Location start(1, 1);
	Location end(1, 18);

	cout << "Generating maze..." << endl;
	Maze maze(MAZE_X, MAZE_Y);

	VisualizeSwitch(algorithm, maze, start, end);
}

/** Renders path through a big maze. */
void BigGraphVisualizer(Algorithm algorithm) {

	MAZE_X = 100;
	MAZE_Y = 100;
	SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
	LevelCell::dims = Vector2f(SIZE, SIZE);
	Location start(1, 1);
	Location end(52, 50);

	cout << "Generating maze..." << endl;
	Maze maze(MAZE_X, MAZE_Y);

	VisualizeSwitch(algorithm, maze, start, end);
}

/** Renders path through a huge maze. */
void HugeGraphVisualizer(Algorithm algorithm) {

	MAZE_X = 200;
	MAZE_Y = 200;
	SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
	LevelCell::dims = Vector2f(SIZE, SIZE);
	Location start(1, 1);
	Location end(100, 100);

	cout << "Generating maze..." << endl;
	Maze maze(MAZE_X, MAZE_Y);

	VisualizeSwitch(algorithm, maze, start, end);
}

/** Returns path from start to end in the graph. */
Path getPath(float mappingScale, Algorithm algorithm, const Level& level, const Graph& graph, const Vector2f& start_, const Vector2f& end_) {
	Location start = mapToLevel(level.rows, mappingScale, start_);
	Location end = mapToLevel(level.rows, mappingScale, end_);
	switch (algorithm) {
		case DIJKSTRA:
			{
				Dijkstra search(graph, start, end);
				return search.search();
			}
		case A_STAR_H1:
			{
				AStar search(graph, start, end, ManhattanHeuristic(end));
				return search.search();
			}
		case A_STAR_H2:
			{
				AStar search(graph, start, end, EuclideanHeuristic(end));
				return search.search();
			}
		case A_STAR_CUSTOM:
			{
				AStar search(graph, start, end, CustomHeuristic(end));
				return search.search();
			}

		default:
			{
				fail("invalid algorithm choice");
				Dijkstra search(graph, start, start);
				return search.search();
			}
	}
}

/** Renders character moving through a level. */
void CharacterGraphVisualizer(Algorithm algorithm) {
	MAZE_X = 22;
	MAZE_Y = 22;
	SIZE = sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
	LevelCell::dims = Vector2f(SIZE, SIZE);

	cout << "Generating level..." << endl;
	Level level = generateCharacterLevel();

	cout << "Generating graph representation of level..." << endl;
	Graph graph = levelToGraph(level, true);
	graph.printy();

	cout << "Generating scene assests..." << endl;
	vector<Crumb> crumbs = vector<Crumb>();
	for (int i = 0; i < NUM_CRUMBS; i++) { crumbs.push_back(Crumb(i, Vector2f(SCENE_WINDOW_X / 2, SCENE_WINDOW_Y / 2))); }
	float scale = 1.5 / SIZE;
	Texture texture;
	texture.loadFromFile("assets/circle.png");
	Character character(&crumbs);
	character.scale = scale;
	character.texture = texture;
	character.sprite = *(new Sprite(texture));
	character.sprite.setScale(scale, scale);
	Vector2f start = mapToWindow(SIZE, Location(1, 1));
	Kinematic initialState;
	initialState.position = start;
	character.setKinematic(initialState);
	character.update(SteeringOutput(), 0, true);
	Mouse mouse;
	Clock clock;

	cout << "Rendering static level pieces..." << endl;
	RenderTexture levelTexture;
	levelTexture.create(SCENE_WINDOW_X, SCENE_WINDOW_Y);
	auto levelSFML = level.toSFML();
	levelTexture.clear(sf::Color{ 255,255,255,0 });
	for (int i = 0; i < level.rows; i++) {
		for (int j = 0; j < level.cols; j++) {
			levelTexture.draw(levelSFML[i][j]);
		}
	}
	levelTexture.display();
	Sprite staticLevel(levelTexture.getTexture());
	Path path;
	Sprite staticPath;
	RenderTexture pathTexture;
	pathTexture.create(SCENE_WINDOW_X, SCENE_WINDOW_Y);
	auto pathSFML = path.toSFML();

	cout << "Rendering level..." << endl;
	SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);
	FollowPath pathFollowing(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
	bool followingPath = false;
	bool newPathExists = false;
	while (sceneView.scene.isOpen()) {
		float dt = clock.restart().asSeconds();
		Event event;
		while (sceneView.scene.pollEvent(event)) {
			switch (event.type) {
				case Event::Closed:
					sceneView.scene.close();
					break;
				case Event::MouseButtonPressed:
					if (!followingPath) {
						cout << "\n\nGetting path..." << endl;
						path = Path();
						path = getPath(SIZE, algorithm, level, graph, character.getPosition(), Vector2f(mouse.getPosition(sceneView.scene)));
						newPathExists = true;
						cout << "Got path." << endl;
						path.print();
						cout << "Explored nodes: " << path.exploredNodes << endl;

						pathFollowing = FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
						followingPath = true;


						pathSFML = path.toSFML();
						pathTexture.clear(sf::Color{ 255,255,255,0 });
						for (const auto& element : pathSFML) { pathTexture.draw(element); }
						pathTexture.display();
						staticPath = Sprite(pathTexture.getTexture());
					}
					break;
			}
		}

		// Re-render scene.
		if (!path.isEmpty()) {
			if (mapToLevel(MAZE_X, SIZE, character.getPosition()) == path.getLast()) {
				followingPath = false;
			}
		}
		if (newPathExists) {
			SteeringOutput acceleration = pathFollowing.calculateAcceleration(character.getKinematic(), Kinematic());
			character.update(acceleration, dt, true);
		}


		sceneView.scene.clear(sf::Color{ 255,255,255,255 });
		sceneView.scene.draw(staticLevel);
		sceneView.scene.draw(staticPath);
		sceneView.scene.draw(character.sprite);
		if (BREAD_CRUMBS) { for (int i = 0; i < crumbs.size(); i++) { crumbs[i].draw(&sceneView.scene); } }
		sceneView.scene.display();
	}
}

/** Prints runtimes of search algorithm to console. */
void Tester(int iterations, Algorithm algorithm, const Graph& graph, const Location& start, const Location& end) {
	switch (algorithm) {
		case DIJKSTRA:
			{
				cout << AlgorithmStrings[0] << " Runtime: " << endl;
				float average = 0;
				float avgExplored = 0;
				for (int i = 0; i < iterations; i++) {
					Dijkstra search(graph, start, end);
					auto start = steady_clock::now();
					Path path = search.search();
					auto end = steady_clock::now();
					auto elapsed = duration_cast<chrono::seconds>(end - start).count();
					cout << "\tIteration " << i << ": " << elapsed << " seconds" << endl;
					cout << "\t             " << path.exploredNodes << " nodes explored" << endl << endl;
					average += elapsed;
					avgExplored += path.exploredNodes;
				}
				average /= iterations;
				avgExplored /= avgExplored;
				cout << "Average runtime over " << iterations << " iterations: " << average << " seconds" << endl;
				cout << "Average memory over " << iterations << " iterations: " << (int)avgExplored << " nodes explored" << endl << endl;
				break;
			}
		case A_STAR_H1:
			{
				cout << AlgorithmStrings[1] << " Runtime: " << endl;
				float average = 0;
				float avgExplored = 0;
				for (int i = 0; i < iterations; i++) {
					AStar search(graph, start, end, ManhattanHeuristic(end));
					auto start = steady_clock::now();
					Path path = search.search();
					auto end = steady_clock::now();
					auto elapsed = duration_cast<chrono::seconds>(end - start).count();
					cout << "\tIteration " << i << ": " << elapsed << " seconds" << endl;
					cout << "\t             " << path.exploredNodes << " nodes explored" << endl << endl;
					average += elapsed;
					avgExplored += path.exploredNodes;
				}
				average /= iterations;
				avgExplored /= avgExplored;
				cout << "Average runtime over " << iterations << " iterations: " << average << " seconds" << endl;
				cout << "Average memory over " << iterations << " iterations: " << (int)avgExplored << " nodes explored" << endl << endl;

				break;
			}
		case A_STAR_H2:
			{
				cout << AlgorithmStrings[2] << " Runtime: " << endl;
				float average = 0;
				float avgExplored = 0;
				for (int i = 0; i < iterations; i++) {
					AStar search(graph, start, end, EuclideanHeuristic(end));
					auto start = steady_clock::now();
					Path path = search.search();
					auto end = steady_clock::now();
					auto elapsed = duration_cast<chrono::seconds>(end - start).count();
					cout << "\tIteration " << i << ": " << elapsed << " seconds" << endl;
					cout << "\t             " << path.exploredNodes << " nodes explored" << endl << endl;
					average += elapsed;
					avgExplored += path.exploredNodes;
				}
				average /= iterations;
				avgExplored /= avgExplored;
				cout << "Average runtime over " << iterations << " iterations: " << average << " seconds" << endl;
				cout << "Average memory over " << iterations << " iterations: " << (int)avgExplored << " nodes explored" << endl << endl;
				break;
			}

		case A_STAR_CUSTOM:
			{
				cout << AlgorithmStrings[3] << " Runtime: " << endl;
				float average = 0;
				float avgExplored = 0;
				for (int i = 0; i < iterations; i++) {
					AStar search(graph, start, end, CustomHeuristic(end));
					auto start = steady_clock::now();
					Path path = search.search();
					auto end = steady_clock::now();
					auto elapsed = duration_cast<chrono::seconds>(end - start).count();
					cout << "\tIteration " << i << ": " << elapsed << " seconds" << endl;
					cout << "\t             " << path.exploredNodes << " nodes explored" << endl << endl;
					average += elapsed;
					avgExplored += path.exploredNodes;
				}
				average /= iterations;
				avgExplored /= avgExplored;
				cout << "Average runtime over " << iterations << " iterations: " << average << " seconds" << endl;
				cout << "Average memory over " << iterations << " iterations: " << (int)avgExplored << " nodes explored" << endl << endl;
				break;
			}
		case INVALID_ALG:
			{
				fail("invalid algorithm choice");
				break;
			}
	}
}

/** Prints runtimes of all search algorithms on big graph to console. */
void Test(int iterations) {
	MAZE_X = 100;
	MAZE_Y = 100;
	Maze maze(MAZE_X, MAZE_Y);
	auto l = maze.getLevel();

	vector<Location> starts;
	starts.push_back(Location(1, 1));
	starts.push_back(Location(52, 50));
	starts.push_back(Location(81, 11));
	starts.push_back(Location(10, 90));
	starts.push_back(Location(98, 98));
	vector<Location> ends;
	ends.push_back(Location(52, 50));
	ends.push_back(Location(1, 1));
	ends.push_back(Location(10, 90));
	ends.push_back(Location(81, 11));
	ends.push_back(Location(1, 1));

	auto algorithms = { Algorithm::DIJKSTRA, Algorithm::A_STAR_H1, Algorithm::A_STAR_H2, Algorithm::A_STAR_CUSTOM };
	for (int i = 0; i < starts.size(); i++) {
		cout << "======================================================" << endl;
		cout << "Locations " << i << endl;
		cout << "Start: (" << starts[i].x << ", " << starts[i].y << ")" << endl;
		cout << "End: (" << ends[i].x << ", " << ends[i].y << ")" << endl;
		for (auto algorithm : algorithms) {
			Tester(iterations, algorithm, maze.getGraph(), starts[i], ends[i]);
		}
		cout << "======================================================" << endl << endl;
	}
}

/** Runs the program.*/
int main(int argc, char* argv[]) {

	srand(1);

	greeting();
	Visualizer visualizer = getVisualizer();
	if (visualizer == Visualizer::test) {
		Test(10);
		exit(0);
	}
	Algorithm algorithm = getAlgorithm();

	switch (visualizer) {
		case Visualizer::small:
			SmallGraphVisualizer(algorithm);
			break;
		case Visualizer::big:
			BigGraphVisualizer(algorithm);
			break;
		case Visualizer::huge:
			HugeGraphVisualizer(algorithm);
			break;
		case Visualizer::character:
			CharacterGraphVisualizer(algorithm);
			break;
		default:
			fail("invalid visualizer choice");
			break;
	}



	return EXIT_SUCCESS;
}