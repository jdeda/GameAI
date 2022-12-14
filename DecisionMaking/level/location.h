#ifndef LOCATION_H
#define LOCATION_H

#include <functional>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

/** X and Y coordinates in a level. */
class Location
{
    public:

    /** Constructs a location with all its fields. */
    Location(int a, int b);

    /** X coordinate in level. */
    int x;

    /** Y coordinate in level. */
    int y;

    /** Support equals operator. */
    inline bool operator==(const Location& other) const {
        return x == other.x && y == other.y;
    }
};



/** Support hashability for Location class.*/
namespace std {
    template <>
    struct hash<Location>
    {
        size_t operator()(const Location& l) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return hash<int>()(l.x) ^ hash<int>()(l.y); // Will this work?
        }
    };
}

/** Returns mapping of position in SFML render window to location in graph. */
Location mapToLevel(int dimension, float mappingScale, const Vector2f& vector);

/** Returns mapping of location in graph to position in SFML render window. */
Vector2f mapToWindow(float mappingScale, const Location& location);

/** Returns Vector2f with x and y flipped. */
Vector2f flip(const Vector2f& v);

/** Returns direction from a to b. (1=left,2=up,3=right,4=down. */
inline int getDirection(const Location& a, const Location& b) {
    // Asssume a and b are not same position.
    // Assume a and b are adjacent.
    if (a.y + 1 == b.y) {
        return 1;
    }
    if (a.x - 1 == b.x) {
        return 2;
    }
    if (a.y - 1 == b.y) {
        return 3;
    }
    if (a.x + 1 == b.x) {
        return 4;
    }
    // exit(69);
    return 0;
}

#endif