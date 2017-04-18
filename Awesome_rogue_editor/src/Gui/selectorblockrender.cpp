#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "selectorblockrender.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Utilities/configs.h"

SelectorBlockRender::SelectorBlockRender()
    : copyBuffer(sf::Vector2u(0, 0))
    , inPast(false)
    , m_startedSelect(false)
{

}

void SelectorBlockRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(inPast && copyBuffer.getSize() != sf::Vector2u(0, 0))
        drawPast(target);
    if(!inPast && m_startedSelect)
        drawSelect(target);
}

void SelectorBlockRender::setPos1(const sf::Vector2u & pos)
{
    m_pos1 = pos;
    m_pos2 = pos;
    if(!inPast)
        m_startedSelect = true;
}

void SelectorBlockRender::setPos2(const sf::Vector2u & pos)
{
    if(!m_startedSelect)
        m_pos1 = pos;
    m_pos2 = pos;
    if(!inPast)
        m_startedSelect = true;
}

void SelectorBlockRender::clear()
{
    m_startedSelect = false;
}

void SelectorBlockRender::setMousePos(const sf::Vector2i & pos)
{
    m_mousePos = pos;
    if(copyBuffer.getSize() == sf::Vector2u(0, 0) && inPast)
        inPast = false;
}

void SelectorBlockRender::drawSelect(sf::RenderTarget & target) const
{
    sf::Vector2f minPos(std::min(m_pos1.x, m_pos2.x)-0.5f, std::min(m_pos1.y, m_pos2.y)-0.5f);
    sf::Vector2f maxPos(std::max(m_pos1.x, m_pos2.x)+0.5f, std::max(m_pos1.y, m_pos2.y)+0.5f);
    minPos *= float(Configs::tiles.tileSize);
    maxPos *= float(Configs::tiles.tileSize);
    sf::RectangleShape sharp(maxPos-minPos);
    sharp.setPosition(minPos);
    sharp.setFillColor(sf::Color(255, 255, 127, generateAlpha()));
    target.draw(sharp);
}

void SelectorBlockRender::drawPast(sf::RenderTarget & target) const
{
    sf::Vector2f topLeftPos((sf::Vector2f(copyBuffer.getSize()/2u))*float(Configs::tiles.tileSize));

    sf::Vector2f mousePos(target.mapPixelToCoords(m_mousePos) + sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f);
    mousePos -= sf::Vector2f(std::fmod(mousePos.x, Configs::tiles.tileSize), std::fmod(mousePos.y, Configs::tiles.tileSize))
            - sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f;
    mousePos -= topLeftPos;

    const sf::Color selectColor(255, 255, 255, generateAlpha());

    sf::RectangleShape sharp(sf::Vector2f(copyBuffer.getSize()) * float(Configs::tiles.tileSize));
    sharp.setPosition(mousePos);
    sharp.setFillColor(selectColor);
    target.draw(sharp);

    if(!Configs::tiles.texture.isValid())
        return;

    sf::VertexArray array(sf::Quads, 8*copyBuffer.getSize().x*copyBuffer.getSize().y);

    for(unsigned int i(0) ; i < copyBuffer.getSize().x ; i++)
        for(unsigned int j(0) ; j < copyBuffer.getSize().y ; j++)
        {
            const Block & b(copyBuffer(sf::Vector2u(i, j)));
            sf::Vector2f pos(mousePos + sf::Vector2f(i, j)*float(Configs::tiles.tileSize));

            drawQuad(&array[8*(i+j*copyBuffer.getSize().x)], sf::FloatRect(pos, sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize))
                    , BlockType::texRect(b.groundID, Configs::tiles.tileSize, Configs::tiles.texture->getSize())
                    , getXFlip(b.groundOrientation), getYFlip(b.groundOrientation), getRotation(b.groundOrientation));

            drawQuad(&array[8*(i+j*copyBuffer.getSize().x)+4], sf::FloatRect(pos, sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize))
                    , BlockType::texRect(b.wallID, Configs::tiles.tileSize, Configs::tiles.texture->getSize())
                    , getXFlip(b.wallOrientation), getYFlip(b.wallOrientation), getRotation(b.wallOrientation));

            for(int k(0) ; k < 8 ; k++)
                array[8*(i+j*copyBuffer.getSize().x)+k].color = selectColor;
        }
    target.draw(array, Configs::tiles.texture());
}

unsigned int SelectorBlockRender::generateAlpha() const
{
    return (std::sin(5*m_clock.getElapsedTime().asSeconds())/10+0.4)*255;
}

sf::IntRect SelectorBlockRender::getSelectedZone() const
{
    if(!m_startedSelect || inPast)
        return sf::IntRect(0, 0, 0, 0);
    sf::Vector2i minPos(std::min(m_pos1.x, m_pos2.x), std::min(m_pos1.y, m_pos2.y));
    sf::Vector2i maxPos(std::max(m_pos1.x, m_pos2.x), std::max(m_pos1.y, m_pos2.y));
    return sf::IntRect(minPos, maxPos-minPos + sf::Vector2i(1,1));
}
