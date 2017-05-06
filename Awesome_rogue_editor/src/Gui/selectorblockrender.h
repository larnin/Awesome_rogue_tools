#ifndef SELECTORBLOCKRENDER_H
#define SELECTORBLOCKRENDER_H

#include "Map/block.h"
#include "Utilities/ressource.h"
#include "Utilities/matrix.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Clock.hpp>


class SelectorBlockRender : public sf::Drawable
{
public:
    SelectorBlockRender();
    virtual ~SelectorBlockRender() = default;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

    void setPos1(const sf::Vector2u & pos);
    void setPos2(const sf::Vector2u & pos);
    void setMousePos(const sf::Vector2i & pos);
    void clear();
    sf::IntRect getSelectedZone() const;

    Matrix<Block> copyBuffer;
    bool inPast;

private:
    void drawSelect(sf::RenderTarget & target) const;
    void drawPast(sf::RenderTarget & target) const;
    unsigned int generateAlpha() const;

    sf::Clock m_clock;
    sf::Vector2i m_mousePos;

    bool m_startedSelect;
    sf::Vector2u m_pos1;
    sf::Vector2u m_pos2;
};

#endif // SELECTORBLOCKRENDER_H
