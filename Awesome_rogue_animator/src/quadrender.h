#ifndef QUADRENDER_H
#define QUADRENDER_H

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>

enum Rotation
{
    ROT_0 = 0,
    ROT_90 = 1,
    ROT_180 = 2,
    ROT_270 = 3
};

void drawQuad(sf::Vertex* quads, const sf::FloatRect & rect, const sf::FloatRect & texRect
              , bool xFliped = false, bool yFliped = false, Rotation rot = Rotation::ROT_0);

#endif // QUADRENDER_H
