#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "../debug/breadcrumbs.h"
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
#include "visualizers.h"

using namespace sf;
using namespace std;

vector<Crumb>* initializeCrumbs() {
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

Character* initializeCharacter(vector<Crumb>* crumbs, Texture* texture) {
    float scale = 1.5 / SIZE;
    Character* character = new Character(crumbs);
    character->scale = scale;
    character->texture = *texture;
    character->sprite = *(new Sprite(*texture));
    character->sprite.setScale(scale, scale);
    Vector2f start = mapToWindow(SIZE, Location(1, 1));
    Kinematic initialState;
    initialState.position = start;
    character->setKinematic(initialState);
    character->update(SteeringOutput(), 0, true);
    return character;
}

Texture* initializeMonsterTexture() {
    Texture* texture = new Texture;
    texture->loadFromFile("assets/monster.png");
    return texture;
}

Character* initializeMonster(vector<Crumb>* crumbs, Texture* texture) {
    // float scale = 1.5 / SIZE;
    float scale = 3 / SIZE;
    Character* character = new Character(crumbs);
    character->scale = scale;
    character->texture = *texture;
    character->sprite = *(new Sprite(*texture));
    character->sprite.setScale(scale, scale);
    Vector2f start = mapToWindow(SIZE, Location(18, 18));
    Kinematic initialState;
    initialState.position = start;
    character->setKinematic(initialState);
    character->update(SteeringOutput(), 0, true);
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

Path getPath(const Environment& environment, const Vector2f& start_, const Vector2f& end_) {
    Location start = mapToLevel(environment.rows, SIZE, start_);
    Location end = mapToLevel(environment.rows, SIZE, end_);
    AStar search(environment.getGraph(), start, end, ManhattanHeuristic(end));
    return search.search();
}