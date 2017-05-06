#ifndef CURSORBLOCKRENDER_H
#define CURSORBLOCKRENDER_H

#include "Map/block.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Clock.hpp>

class CursorBlockRender : public sf::Drawable
{
public:
    CursorBlockRender();
    virtual ~CursorBlockRender() = default;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

    void setMousePos(const sf::Vector2i & pos);

    Block block;

    bool drawWall;
    bool drawGround;
    bool drawTop;

private:
    sf::Clock m_clock;
    sf::Vector2i m_mousePos;
};

#endif // CURSORBLOCKRENDER_H
