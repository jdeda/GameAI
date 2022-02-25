#include <SFML/Graphics.hpp>
#include "../hparams/hyperparameters.h"
#include "../steering/steeringoutput.h"
#include "kinematic.h"

using namespace sf;

        Kinematic::Kinematic() {
            position = Vector2f(0.f, 0.f);
            orientation = 0.f;
            linearVelocity = Vector2f(0.f, 0.f);
            angularVelocity = 0.f;
        }
