#include "quadrender.h"

void drawQuad(sf::Vertex* quads, const sf::FloatRect & rect, const sf::FloatRect & texRect, bool xFliped, bool yFliped, Rotation rot)
{
    quads[0] = sf::Vertex(sf::Vector2f(rect.left, rect.top), sf::Vector2f(texRect.left, texRect.top));
    quads[1] = sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top), sf::Vector2f(texRect.left+texRect.width, texRect.top));
    quads[2] = sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top+rect.height), sf::Vector2f(texRect.left+texRect.width, texRect.top+texRect.height));
    quads[3] = sf::Vertex(sf::Vector2f(rect.left, rect.top+rect.height), sf::Vector2f(texRect.left, texRect.top+texRect.height));

    for(unsigned int i(0) ; i < rot ; i++)
    {
        sf::Vector2f texTemp(quads[0].texCoords);
        quads[0].texCoords = quads[1].texCoords;
        quads[1].texCoords = quads[2].texCoords;
        quads[2].texCoords = quads[3].texCoords;
        quads[3].texCoords = texTemp;
    }

    if(xFliped)
    {
        std::swap(quads[0].texCoords, quads[1].texCoords);
        std::swap(quads[2].texCoords, quads[3].texCoords);
    }

    if(yFliped)
    {
        std::swap(quads[0].texCoords, quads[3].texCoords);
        std::swap(quads[1].texCoords, quads[2].texCoords);
    }
}



