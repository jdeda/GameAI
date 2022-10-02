#ifndef VMATH_H
#define VMATH_H

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

namespace vmath {

    /** Returns dot product of the two given vectors. */
    inline float dot_product(const Vector2f& v1, const Vector2f& v2) {
        return (v1.x * v2.x) + (v1.y * v2.y);
    }

    /** Returns length of the given vector */
    inline float length(const Vector2f& v) {
        return sqrt(dot_product(v, v));
    }

    /** Returns normalized vector (magnitude of 1) of given one. */
    inline Vector2f normalized(const Vector2f& v) {
        Vector2f z = v;
        float l = sqrt(dot_product(z, z));
        if (l != 0) {
            z /= l;
        }
        return z;
    }

    /** Returns vector representation of the given orientation (given in angles).*/
    inline Vector2f asVector(const float angle) {
        return Vector2f(cos(angle * (M_PI / 180.f)), sin(angle * (M_PI / 180.f)));
    }
};

#endif
