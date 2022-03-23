#ifndef BCRUMBS_H
#define BCRUMBS_H

#include <SFML/Graphics.hpp>

using namespace sf;

const int NUM_CRUMBS = 10;

class Crumb : sf::CircleShape
{

private:
    int id;

public:
    Crumb(const int id, const Vector2f& pos);
    void drop(float x, float y);
    void drop(Vector2f position);
    void draw(RenderWindow* window);
    Vector2f pos();
};

#endif
