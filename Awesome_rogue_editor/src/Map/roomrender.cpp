#include <SFML/Graphics/RectangleShape.hpp>
#include "roomrender.h"
#include "Utilities/quadrender.h"
#include "blocktype.h"
#include "Utilities/configs.h"

RoomRender::RoomRender(std::weak_ptr<Patern> room)
    : m_render(sf::Quads)
    , m_room(room)
    , m_showWallsBoxs(false)
    , m_wallsRender(sf::Lines)
{
    redraw();
}

void RoomRender::redraw()
{
    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
    {
        m_render.resize(0);
        return;
    }
    m_render.resize(getNbSurfaces()*4);

    int index(0);
    for(unsigned int i(0) ; i < r->getSize().x ; i++)
        for(unsigned int j(0) ; j < r->getSize().y ; j++)
        {
            const Block &b((*r)(sf::Vector2u(i, j)));
            index += drawBlock(&m_render[index*4], b, sf::Vector2i(i, j));
        }

    if(m_showWallsBoxs)
        redrawWalls();
}

void RoomRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    std::shared_ptr<Patern> r(m_room.lock());
    if(r)
    {
        sf::RectangleShape sharp(sf::Vector2f(r->getSize())*float(Configs::tiles.tileSize));
        sharp.setFillColor(sf::Color(255, 255, 255, 50));
        sharp.move(Configs::tiles.tileSize/-2.0f, Configs::tiles.tileSize/-2.0f);
        target.draw(sharp);

        if(Configs::tiles.texture.isValid())
            target.draw(m_render, sf::RenderStates(Configs::tiles.texture()));

        if(m_showWallsBoxs)
            target.draw(m_wallsRender);
    }
}

unsigned int RoomRender::getNbSurfaces() const
{
    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
        return 0;

    int nbDrawable(0);
    for(unsigned int i(0) ; i < r->getSize().x ; i++)
        for(unsigned int j(0) ; j < r->getSize().y ; j++)
        {
            const Block & b((*r)(sf::Vector2u(i, j)));
            if(b.groundID != 0)
                nbDrawable++;
            if(b.wallID != 0)
                nbDrawable++;
        }
    return nbDrawable;
}

unsigned int RoomRender::drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const
{
    unsigned int offset(0);

    float tileSize(Configs::tiles.tileSize);

    if(b.groundID != 0)
    {
        sf::FloatRect rect(BlockType::texRect(b.groundID, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
        drawQuad(quads, sf::FloatRect(sf::Vector2f(globalPos)*tileSize-sf::Vector2f(tileSize, tileSize)/2.0f, sf::Vector2f(tileSize, tileSize))
                 , rect, getXFlip(b.groundOrientation), getYFlip(b.groundOrientation), getRotation(b.groundOrientation));
        offset++;
    }
    if(b.wallID != 0)
    {
        sf::FloatRect rect(BlockType::texRect(b.wallID, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
        drawQuad(quads+4*offset, sf::FloatRect(sf::Vector2f(globalPos)*tileSize-sf::Vector2f(tileSize, tileSize)/2.0f, sf::Vector2f(tileSize, tileSize))
                 , rect, getXFlip(b.wallOrientation), getYFlip(b.wallOrientation), getRotation(b.wallOrientation));
        offset++;
    }

    return offset;
}

std::weak_ptr<Patern> RoomRender::getRoom() const
{
    return m_room;
}

void RoomRender::showWallsBoxs(bool value)
{
    m_showWallsBoxs = value;
    if(!value)
        m_wallsRender.resize(0);
    else redrawWalls();
}

void RoomRender::redrawWalls()
{
    m_wallsRender.clear();

    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
        return;

    for(unsigned int i(0) ; i < r->getSize().x ; i++)
        for(unsigned int j(0) ; j < r->getSize().y ; j++)
        {
            const Block &b((*r)(sf::Vector2u(i, j)));
            HitBox box(BlockType::createBox(b.boxCaracts));
            for(const Line & l : box.lines)
            {
                m_wallsRender.append(sf::Vertex((l.pos1+sf::Vector2f(i, j))*float(Configs::tiles.tileSize), sf::Color::Yellow));
                m_wallsRender.append(sf::Vertex((l.pos2+sf::Vector2f(i, j))*float(Configs::tiles.tileSize), sf::Color::Yellow));
            }
        }
}
