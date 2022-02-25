#ifndef VMATH_H
#define VMATH_H

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

namespace vmath
{

    /** Returns dot product of the two given vectors. */
    inline float dot_product(const Vector2f &v1, const Vector2f &v2);

    /** Returns length of the given vector */
    inline float length(const Vector2f &v);

    /** Returns normalized vector (magnitude of 1) of given one. */
    inline Vector2f normalized(const Vector2f &v);

    /** Returns vector representation of the given orientation (given in angles).*/
    inline Vector2f asVector(const float angle);
};

#endif
