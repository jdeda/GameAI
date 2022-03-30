#include <SFML/Graphics.hpp>
#include "../hparams/hyperparameters.h"
#include "../environment/environment.h"
#include "../steering/steering.h"
#include "../search/a*.h"
#include "../search/search.h"
#include "../scene/scene.h"
#include "../decision/decisiontree.h"
#include "visualizers.h"

using namespace sf;
using namespace std;

void DecisionTreeVisualizer() {

    // Create all assests.
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
    bool* followingPath = new bool(false);
    bool* newPathExists = new bool(false);
    bool* monsterClose = new bool(false);

    // SceneView assets.
    SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);
    Clock clock;
    Mouse mouse;

    // DecisionTree state.
    Location* mouseLocation = new Location(mapToLevel(MAZE_X, SIZE, Vector2f(mouse.getPosition())));
    float *dt = new float(0.f);

    // DecisionTree.
    CharacterDecisionTree tree(environment.getGraph(), character, mouseLocation, dt, monsterClose, followingPath);
    cout << "ok" << endl;


    // Animate.
    cout << "Rendering level..." << endl;
    while (sceneView.scene.isOpen()) {
        *dt = clock.restart().asSeconds();

        // Handle scene poll event.
        Event event;
        while (sceneView.scene.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    sceneView.scene.close();
                    break;
                case Event::MouseButtonPressed:
                    if (*followingPath == false) {
                        cout << "hi!" << endl;
                        //  Get path and path following.
                        // TODO: do these do anything? pointer dumb.
                        *mouseLocation = mapToLevel(MAZE_X, SIZE, Vector2f(mouse.getPosition()));
                        *newPathExists = true;
                        *followingPath = true;
                    }
                    break;
            }
        }
        // Mark followingPath.
        if (!path.isEmpty() && mapToLevel(MAZE_X, SIZE, character->getPosition()) == path.getLast()) {
            *followingPath = false;
        }

        // DecisionTree makes decision.
        tree.makeDecision();

        // Update path if tree makes one.
        if(!tree.getPath().isEmpty()) {
            pathSFML = tree.getPath().toSFML();
            pathTexture.clear(sf::Color{ 255,255,255,0 });
            for (const auto& element : pathSFML) { pathTexture.draw(element); }
            pathTexture.display();
            pathSprite = Sprite(pathTexture.getTexture());
        }

        // Re-draw scene.
        sceneView.scene.clear(sf::Color{ 255,255,255,255 });
        sceneView.scene.draw(levelSprite);
        sceneView.scene.draw(pathSprite);
        sceneView.scene.draw(character->sprite);
        sceneView.scene.display();
    }

    // Free any allocated memory.
    delete levelTexture;
    delete characterCrumbs;
    delete characterTexture;
    delete character;
    delete followingPath;
    delete newPathExists;
    delete monsterClose;
    delete dt;
}