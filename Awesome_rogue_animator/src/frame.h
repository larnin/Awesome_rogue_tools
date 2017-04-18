#ifndef FRAME_H
#define FRAME_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

struct Frame
{
    inline Frame()
        : time(0)
    {}

    inline Frame(float _time, const sf::FloatRect & _texRect, const sf::Vector2f & _offset)
        : time(_time), texRect(_texRect), offset(_offset)
    {}

    float time;
    sf::FloatRect texRect;
    sf::Vector2f offset;
};

#endif // FRAME_H
