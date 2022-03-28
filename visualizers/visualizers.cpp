#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "../debug/breadcrumbs.h"
#include "../userinput/userinput.h"
#include "../scene/scene.h"
#include "../character/character.h"
#include "../kinematic/kinematic.h"
#include "../hparams/hyperparameters.h"
#include "../steering/steering.h"
#include "../steering/steeringoutput.h"
#include "../graph/graph.h"
#include "../level/level.h"
#include "../environment/environment.h"
#include "../search/search.h"
#include "../search/a*.h"
#include "../decision/decisiontree.h"
#include "../decision/behaviortree.h"
#include "../decision/decisiontreelearning.h"
#include "visualizers.h"

using namespace sf;
using namespace std;

float MAZE_X = 22.f;
float MAZE_Y = 22.f;
float SIZE = std::sqrt((SCENE_WINDOW_X * SCENE_WINDOW_Y) / (MAZE_X * MAZE_Y));
const bool BREAD_CRUMBS = false;
int ID::count = 0;
Vector2f LevelCell::dims = Vector2f(SIZE, SIZE);

vector<Crumb>* initializeCharacterCrumbs() {
    vector<Crumb>* crumbs = new vector<Crumb>();
    for (int i = 0; i < NUM_CRUMBS; i++) { 
        crumbs->push_back(Crumb(i, Vector2f(SCENE_WINDOW_X / 2, SCENE_WINDOW_Y / 2)));
    }
    return crumbs;
}

Texture* initializeCharacterTexture() {
    Texture* texture = new Texture;
    texture->loadFromFile("assets/circle.png");
    return texture;
}

Character initializeCharacter(vector<Crumb>* crumbs, Texture* texture) {
    float scale = 1.5 / SIZE;
    Character character(crumbs);
    character.scale = scale;
    character.texture = *texture;
    character.sprite = *(new Sprite(*texture));
    character.sprite.setScale(scale, scale);
    Vector2f start = mapToWindow(SIZE, Location(1, 1));
    Kinematic initialState;
    initialState.position = start;
    character.setKinematic(initialState);
    character.update(SteeringOutput(), 0, true);
    return character;
}

RenderTexture* generateLevelTexture(const Level& level) {
    RenderTexture* levelTexture = new RenderTexture;
    levelTexture->create(SCENE_WINDOW_X, SCENE_WINDOW_Y);
    auto levelSFML = level.toSFML();
    levelTexture->clear(sf::Color{ 255,255,255,0 });
    for (int i = 0; i < level.rows; i++) {
        for (int j = 0; j < level.cols; j++) {
            levelTexture->draw(levelSFML[i][j]);

        }
    }
    levelTexture->display();
    return levelTexture;
}

Path getPath(const Level& level, const Graph& graph, const Vector2f& start_, const Vector2f& end_) {
    Location start = mapToLevel(level.rows, SIZE, start_);
	Location end = mapToLevel(level.rows, SIZE, end_);
    AStar search(graph, start, end, ManhattanHeuristic(end));
    return search.search();
}

void DecisionTreeVisualizer() {

    // Create visualization assests.
    cout << "Creating scene assets..." << endl;
    Environment environment;

    // Character assets.
    auto characterCrumbs = initializeCharacterCrumbs();
    auto characterTexture = initializeCharacterTexture();
    auto character = initializeCharacter(characterCrumbs, characterTexture);

    // Level assests.
    auto levelTexture = generateLevelTexture(environment.getLevel());
    Sprite levelSprite(levelTexture->getTexture());

    // Path.
    Path path;
    RenderTexture pathTexture;
    pathTexture.create(SCENE_WINDOW_X, SCENE_WINDOW_Y);
    vector<LevelCell> pathSFML;
    Sprite pathSprite;

    // Path following.
    FollowPath pathFollowing(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
    bool followingPath = false;
    bool newPathExists = false;

    // SceneView assets.
    SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);
    Clock clock;
    Mouse mouse;

    cout << "Rendering level..." << endl;
    while (sceneView.scene.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Handle scene poll event.
        Event event;
        while (sceneView.scene.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    sceneView.scene.close();
                    break;
                case Event::MouseButtonPressed:
                    if (!followingPath) {
                        //  Get path and path following.
                        path = getPath(environment.getLevel(), environment.getGraph(), character.getPosition(), Vector2f(mouse.getPosition(sceneView.scene)));
                        pathFollowing = FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
                        newPathExists = true;
                        followingPath = true;

                        // Recreate path sprite.
                        pathSFML = path.toSFML();
                        pathTexture.clear(sf::Color{ 255,255,255,0 });
                        for (const auto& element : pathSFML) { pathTexture.draw(element); }
                        pathTexture.display();
                        pathSprite = Sprite(pathTexture.getTexture());
                    }
                    break;
            }
        }
        // Mark followingPath.
        if (!path.isEmpty() && mapToLevel(MAZE_X, SIZE, character.getPosition()) == path.getLast()) {
            followingPath = false;
        }

        // Update if and only if newPathExists.
        if (newPathExists) {
            SteeringOutput pathAcceleration = pathFollowing.calculateAcceleration(character.getKinematic(), Kinematic());
            character.update(pathAcceleration, dt, true);
        }

        // Re-draw scene.
        sceneView.scene.clear(sf::Color{ 255,255,255,255 });
        sceneView.scene.draw(levelSprite);
        sceneView.scene.draw(pathSprite);
        sceneView.scene.draw(character.sprite);
        sceneView.scene.display();
    }

    // Free any allocated memory.
    delete levelTexture;
    delete characterCrumbs;
    delete characterTexture;
    // delete character;
}

void BehaviorTreeVisualizer() {}

void DecisionTreeLearningVisualizer() {}