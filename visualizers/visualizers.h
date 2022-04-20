#ifndef VISUALIZERS_H
#define VISUALIZERS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "../character/character.h"
#include "../hparams/hyperparameters.h"
#include "../search/search.h"
#include "../environment/environment.h"

using namespace sf;
using namespace std;

vector<Crumb>* initializeCrumbs();

Texture* initializeCharacterTexture();

Character* initializeCharacter(vector<Crumb>* crumbs, Texture* texture);

Texture* initializeMonsterTexture();

Character* initializeMonster(vector<Crumb>* crumbs, Texture* texture);

RenderTexture* generateLevelTexture(const Level& level);

Path getPath(const Environment& environment, const Vector2f& start_, const Vector2f& end_);


// TODO: This is really a hack...current pathfinding does not work like this so it will get stuck...
inline bool closeEnough(const Vector2f& v1, const Vector2f& v2) {
    float satisfaction = (SIZE / 2.f) / 4.f;
    bool xSatisfied = (v1.x <= v2.x + satisfaction) && (v1.x >= v2.x - satisfaction);
    bool ySatisfied = (v1.y <= v2.y + satisfaction) && (v1.y >= v2.y - satisfaction);
    // cout << "s: " << satisfaction << endl;
    // cout << "v1: " << v1.x << " " << v1.y << endl;
    // cout << "v2: " << v2.x << " " << v2.y << endl;
    return xSatisfied && ySatisfied;
}

inline bool snatched(Character* c1, Character* c2) {
    auto a = c1->getLocation();
    auto b = c2->getLocation();
    cout << "SNATCHED? " << endl;
    cout << a.x << " " << a.y << endl;
    cout << b.x << " " << b.y << endl << endl;
    return a == b;
}

#endif