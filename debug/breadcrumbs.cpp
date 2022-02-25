#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "breadcrumbs.h"

using namespace sf;

Crumb::Crumb(const int id, const Vector2f &pos)
{
    this->id = id;
    this->setRadius(5.f);
    this->setFillColor(sf::Color(0, 0, 255, 255));
    this->setPosition(pos);
}
void Crumb::drop(float x, float y)
{
    this->setPosition(x, y);
}
void Crumb::drop(sf::Vector2f position)
{
    this->setPosition(position);
}
void Crumb::draw(sf::RenderWindow *window)
{
    window->draw(*this);
}

Vector2f Crumb::pos() { return this->getPosition(); }