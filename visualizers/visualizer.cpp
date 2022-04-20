#include <SFML/Graphics.hpp>
#include "../hparams/hyperparameters.h"
#include "../environment/environment.h"
#include "../steering/steering.h"
#include "../search/a*.h"
#include "../search/search.h"
#include "../scene/scene.h"
#include "../decision/decisiontree.h"
#include "../decision/behaviortree.h"
#include "visualizers.h"

using namespace sf;
using namespace std;

void Visualizer() {

    // Create all assests.
    cout << "Creating scene assets..." << endl;
    Environment environment;

    // Character assets.
    auto characterCrumbs = initializeCrumbs();
    auto characterTexture = initializeCharacterTexture();
    auto character = initializeCharacter(characterCrumbs, characterTexture);

    // Monster assets.
    auto monsterCrumbs = initializeCrumbs();
    auto monsterTexture = initializeMonsterTexture();
    auto monster = initializeMonster(monsterCrumbs, monsterTexture);

    // Level assests.
    auto levelTexture = generateLevelTexture(environment.getLevel());
    Sprite levelSprite(levelTexture->getTexture());

    // Path textures.
    RenderTexture pathTexture;
    pathTexture.create(SCENE_WINDOW_X, SCENE_WINDOW_Y);
    vector<LevelCell> pathSFML;
    Sprite pathSprite;

    // Path textures.
    RenderTexture mpathTexture;
    mpathTexture.create(SCENE_WINDOW_X, SCENE_WINDOW_Y);
    vector<LevelCell> mpathSFML;
    Sprite mpathSprite;

    // Path following.
    FollowPath pathFollowing(Path(), PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
    bool* followingPath = new bool(false);
    bool* newPathExists = new bool(false);
    bool* monsterClose = new bool(false);

    // SceneView assets.
    SceneView sceneView(SCENE_WINDOW_X, SCENE_WINDOW_Y, SCENE_WINDOW_FR);
    Clock clock;
    Mouse mouse;

    // Additional state for DecisionTree.
    Location* mouseLocation = new Location(mapToLevel(MAZE_X, SIZE, Vector2f(mouse.getPosition())));
    float* dt = new float(0.f);

    // DecisionTree.
    CharacterDecisionTree characterTree(environment.getGraph(), character, mouseLocation, dt, monsterClose, followingPath);

    // BehaviorTree.
    MonsterBehaviorTree monsterTree(environment.getGraph(), character, monster, dt);

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
                        // TODO: Inverse the mouse location or the visualizer?
                        *mouseLocation = mapToLevel(MAZE_X, SIZE, Vector2f(mouse.getPosition(sceneView.scene)));
                        *newPathExists = true;
                        *followingPath = true;
                    }
                    break;
            }
        }

        // DecisionTree makes decision.
        characterTree.makeDecision();

        // Mark followingPath (decision tree).
        if (*followingPath) {
            if (!characterTree.getPath().isEmpty()) {
                auto path = characterTree.getPath();
                auto a = path.getPathList()[path.size() - 1].getLocation();
                auto b = path.getPathList()[path.size() - 2].getLocation();
                auto d = getDirection(a, b);
                auto t = flip(mapToWindow(SIZE, path.getLast()));
                if (d == 3) { t.x += 6; }
                if (d == 2) { t.y += 6; }
                if (closeEnough(character->getPosition(), t)) {
                    *followingPath = false;
                }
            }
        }

        // Update path first iteration of tree path finding (decision tree).
        if (*followingPath) {
            if (!characterTree.getPath().isEmpty() && characterTree.getFollowingIteration() == 1) {
                pathSFML = characterTree.getPath().toSFML();
                pathTexture.clear(sf::Color{ 255,255,255,0 });
                for (const auto& element : pathSFML) { pathTexture.draw(element); }
                pathTexture.display();
                pathSprite = Sprite(pathTexture.getTexture());
            }
        }

        // Behavior tree makes decision.
        monsterTree.run();

        if (monsterTree.isChasing) {
            if (!monsterTree.getPath().isEmpty()) {
                auto path = monsterTree.getPath();
                auto a = path.getPathList()[path.size() - 1].getLocation();
                auto b = path.getPathList()[path.size() - 2].getLocation();
                auto d = getDirection(a, b);
                auto t = flip(mapToWindow(SIZE, path.getLast()));
                if (d == 3) { t.x += 6; }
                if (d == 2) { t.y += 6; }
                if (closeEnough(monster->getPosition(), t)) {
                    monsterTree.isChasing = false;
                }
            }
        }
        if (monsterTree.isChasing) {
            if (!monsterTree.getPath().isEmpty() && monsterTree.getChasingIteration() == 1) {
                mpathSFML = monsterTree.getPath().toSFML();
                mpathTexture.clear(sf::Color{ 255,255,255,0 });
                for (const auto& element : mpathSFML) { mpathTexture.draw(element); }
                mpathTexture.display();
                mpathSprite = Sprite(mpathTexture.getTexture());
            }
        }

        cout << "POS: " << character->getPosition().x << " " << character->getPosition().y << endl;

        // Re-draw scene.
        sceneView.scene.clear(sf::Color{ 255,255,255,255 });
        sceneView.scene.draw(levelSprite);
        if (*followingPath) { sceneView.scene.draw(pathSprite); }
        if (monsterTree.isChasing) { sceneView.scene.draw(mpathSprite); }
        sceneView.scene.draw(character->sprite);
        sceneView.scene.draw(monster->sprite);
        sceneView.scene.display();
    }

    // Free any allocated memory.
    delete levelTexture;
    delete characterCrumbs;
    delete characterTexture;
    delete character;
    delete monsterCrumbs;
    delete monsterTexture;
    delete monster;
    delete followingPath;
    delete newPathExists;
    delete monsterClose;
    delete dt;
}