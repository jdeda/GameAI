#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace sf;

const sf::Vector2f TOP_RIGHT = sf::Vector2f(550, 0);
const sf::Vector2f BOT_RIGHT = sf::Vector2f(550, 550);
const sf::Vector2f BOT_LEFT = sf::Vector2f(0, 550);
const sf::Vector2f TOP_LEFT = sf::Vector2f(0, 0);

const int NUM_CRUMBS = 15;


class Crumb: sf::CircleShape {

    private:
        int id;

    public:
        Crumb(const int id, const Vector2f& pos)
        {
            this->id = id;         
            this->setRadius(5.f);
            this->setFillColor(sf::Color(0, 0, 255, 255));
            this->setPosition(pos);
        }
        void drop(float x, float y)
        {
            this->setPosition(x, y);
        }
        void drop(sf::Vector2f position)
        {
            this->setPosition(position);
        }
        void draw(sf::RenderWindow* window)
        {
            window->draw(*this);
        }

        Vector2f pos() { return this->getPosition(); }
};