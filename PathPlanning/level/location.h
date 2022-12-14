#ifndef LOCATION_H
#define LOCATION_H

#include <functional>
#include <SFML/Graphics.hpp>

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

#endif