#ifndef QUADRENDER_H
#define QUADRENDER_H

#include "Map/block.h"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>

void drawQuad(sf::Vertex* quads, sf::FloatRect rect, sf::FloatRect texRect, bool xFliped = false, bool yFliped = false, Rotation rot = ROT_0);

#endif // QUADRENDER_H
