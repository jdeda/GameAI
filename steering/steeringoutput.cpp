#include <SFML/Graphics.hpp>
#include "steeringoutput.h"

using namespace sf;

Vector2f linearAcceleration;
float angularAcceleration;

SteeringOutput::SteeringOutput()
{
    this->linearAcceleration = Vector2f(0.f, 0.f);
    this->angularAcceleration = 0.f;
}
