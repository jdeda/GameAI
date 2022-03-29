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

#endif