#include "cursorblockrender.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "Utilities/configs.h"

CursorBlockRender::CursorBlockRender()
    : drawWall(true)
    , drawGround(true)
{

}

void CursorBlockRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(Configs::tiles.texture.isValid())
    {
        sf::Vector2f mousePos(target.mapPixelToCoords(m_mousePos) + sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f);
        mousePos = sf::Vector2f(mousePos) - sf::Vector2f(std::fmod(mousePos.x, Configs::tiles.tileSize), std::fmod(mousePos.y, Configs::tiles.tileSize))
                - sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f;

        sf::VertexArray array(sf::Quads, 0);

        if(drawGround)
        {
            array.resize(4);
            sf::FloatRect rect(BlockType::texRect(block.groundID, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
            drawQuad(&array[0], sf::FloatRect(mousePos, sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)), rect
                    , getXFlip(block.groundOrientation), getYFlip(block.groundOrientation), getRotation(block.groundOrientation));
        }
        if(drawWall)
        {
            array.resize(array.getVertexCount() + 4);
            sf::FloatRect rect(BlockType::texRect(block.wallID, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
            drawQuad(&array[array.getVertexCount()-4], sf::FloatRect(mousePos, sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)), rect
                    , getXFlip(block.wallOrientation), getYFlip(block.wallOrientation), getRotation(block.wallOrientation));
        }

        float alpha(std::sin(10*m_clock.getElapsedTime().asSeconds())/8+0.4);

        for(unsigned int i(0) ; i < array.getVertexCount() ; i++)
            array[i].color.a = alpha*255;

        sf::RectangleShape rect(sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize));
        rect.setPosition(mousePos);
        rect.setFillColor(sf::Color(255, 255, 255, alpha*255));
        target.draw(rect);
        target.draw(array, sf::RenderStates(Configs::tiles.texture()));
    }
}

void CursorBlockRender::setMousePos(const sf::Vector2i & pos)
{
    m_mousePos = pos;
}
