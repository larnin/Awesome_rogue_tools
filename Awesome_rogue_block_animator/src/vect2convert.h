#ifndef VECT2CONVERT_H
#define VECT2CONVERT_H

#include <cmath>
#include <SFML/System/Vector2.hpp>

inline float angle(const sf::Vector2f & vect)
{
    return atan2(vect.y,vect.x);
}

inline float norm(const sf::Vector2f & vect)
{
    return sqrt(vect.x*vect.x +vect.y*vect.y);
}

inline sf::Vector2f normalise(const sf::Vector2f & vect)
{
    return vect/norm(vect);
}

inline sf::Vector2f toVect(float norm, float angle)
{
    return sf::Vector2f(cos(angle)*norm, sin(angle)*norm);
}

inline sf::Vector2f rotate(const sf::Vector2f & vect, float rotateAngle)
{
    return toVect(norm(vect), angle(vect) + rotateAngle);
}

#endif // VECT2CONVERT_H
