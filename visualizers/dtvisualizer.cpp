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
                        path = getPath(environment, character.getPosition(), Vector2f(mouse.getPosition(sceneView.scene)));
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