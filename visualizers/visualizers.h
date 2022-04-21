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
    return xSatisfied && ySatisfied;
}

/** c1=monster*/
inline bool snatched(Character* c1, Character* c2) {
    auto a1 = c1->getLocation();
    a1.x += 1;
    a1.y += 1;
    auto a2 = a1;
    a2.y -=  1;
    auto b = c2->getLocation();
    // cout << "SNATCHED? " << endl;
    // cout << a1.x << " " << a1.y << endl;
    // cout << a2.x << " " << a2.y << endl;
    // cout << b.x << " " << b.y << endl << endl;
    return (a1 == b) || (a2 == b);
}

inline float distance(Character* character, Character* monster) {
    auto a = character->getPosition();
    auto b = monster->getPosition();
    auto d = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    return d;
}
#endif