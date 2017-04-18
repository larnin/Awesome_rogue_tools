#include "grid.h"
#include "Map/blocktype.h"

Grid::Grid()
    : m_color(sf::Color::White)
    , m_size(0, 0)
    , m_render(sf::Lines, 0)
    , m_tileSize(16)
    , m_delta(8)
{

}

void Grid::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    target.draw(m_render);
}

void Grid::setSize(const sf::Vector2u & size)
{
    m_size = size;
    generateRender();
}

void Grid::setColor(const sf::Color & color)
{
    m_color = color;
    generateRender();
}

void Grid::setTileSize(unsigned int size)
{
    m_tileSize = size;
    generateRender();
}

void Grid::setDelta(float delta)
{
    m_delta = delta;
    generateRender();
}

void Grid::generateRender()
{
    const sf::Vector2f deltaPos(m_delta, m_delta);

    m_render.resize(2*(m_size.x+m_size.y+2));
    for(unsigned int i(0) ; i <= m_size.x ; i++)
    {
        m_render[2*i].position = sf::Vector2f(i*m_tileSize, 0) - deltaPos;
        m_render[2*i].color = m_color;
        m_render[2*i+1].position = sf::Vector2f(i*m_tileSize, m_size.y*m_tileSize) - deltaPos;
        m_render[2*i+1].color = m_color;
    }
    const int delta(2*(m_size.x + 1));
    for(unsigned int i(0) ; i <= m_size.y ; i++)
    {
        m_render[2*i+delta].position = sf::Vector2f(0, i*m_tileSize) - deltaPos;
        m_render[2*i+delta].color = m_color;
        m_render[2*i+1+delta].position = sf::Vector2f(m_size.x*m_tileSize, i*m_tileSize) - deltaPos;
        m_render[2*i+1+delta].color = m_color;
    }
}
