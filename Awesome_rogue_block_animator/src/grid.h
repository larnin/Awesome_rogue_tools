#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Grid : public sf::Drawable
{
public:
    Grid();
    virtual ~Grid() = default;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

    void setSize(const sf::Vector2u & size);
    void setColor(const sf::Color & color);
    void setTileSize(unsigned int size);
    void setDelta(float delta);

private:
    void generateRender();

    sf::Color m_color;
    sf::Vector2u m_size;
    sf::VertexArray m_render;
    unsigned int m_tileSize;
    float m_delta;
};

#endif // GRID_H
