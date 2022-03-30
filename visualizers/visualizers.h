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

vector<Crumb>* initializeCharacterCrumbs();

Texture* initializeCharacterTexture();

Character* initializeCharacter(vector<Crumb>* crumbs, Texture* texture);

RenderTexture* generateLevelTexture(const Level& level);

Path getPath(const Environment& environment, const Vector2f& start_, const Vector2f& end_);


// TODO: This is really a hack...current pathfinding does not work like this so it will get stuck...
inline bool closeEnough(const Vector2f& v1_, const Vector2f& v2) {
    Vector2f v1 = v1_;
    v1.x = v1_.y;
    v1.y = v1_.x;
    float satisfaction = (SIZE / 2.f) / 3.f;
    bool xSatisfied = (v1.x <= v2.x + satisfaction) && (v1.x >= v2.x - satisfaction); 
    bool ySatisfied = (v1.y <= v2.y + satisfaction) && (v1.y >= v2.y - satisfaction);
    return xSatisfied && ySatisfied;
}
#endif