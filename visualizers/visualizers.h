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

inline bool closeEnough(const Vector2f& v1_, const Vector2f& v2) {
    Vector2f v1 = v1_;
    v1.x = v1_.y;
    v1.y = v1_.x;
    float satisfaction = (SIZE / 2.f) / 2.f;
    cout << satisfaction << endl;
    bool xSatisfied = (v1.x <= v2.x + satisfaction) && (v1.x >= v2.x - satisfaction); 
    bool ySatisfied = (v1.y <= v2.y + satisfaction) && (v1.y >= v2.y - satisfaction);
    // cout << "v1: "<< v1.x << " " << v1.y << endl; 
    // cout << "v2: "<< v2.x << " " << v2.y << endl << endl;
    return xSatisfied && ySatisfied;
    // bool xSatisfied = vector1.x <= vector2.x + satisfaction || vector1.x >= vector2.x - satisfaction;
    // bool xSatisfied = vector1.x == vector2.x + satisfaction || vector1.x == vector2.x - satisfaction;
    // bool ySatisfied = vector1.y == vector2.y + satisfaction || vector1.y == vector2.y - satisfaction;
    // return xSatisfied && ySatisfied;
}
#endif