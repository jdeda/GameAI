#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;


namespace vmath {
    /** Returns dot product of the two given vectors. */
    inline float dot(const Vector2f& v1, const Vector2f& v2)
    {
        return (v1.x * v2.x) + (v1.y * v2.y);
    }

    /** Returns length of the given vector */
    inline float length(const Vector2f& v)
    {
        return sqrt(dot(v, v));
    }

    /** Returns normalized vector (magnitude of 1) of given one. */
    inline Vector2f normalized(const Vector2f& v)
    {
        Vector2f z = v;
        float l = sqrt(dot(z, z));
        if (l != 0) {
            z /= l;
        }
        return z;
    }
};