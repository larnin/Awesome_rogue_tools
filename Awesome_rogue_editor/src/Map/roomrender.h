#ifndef ROOMRENDER_H
#define ROOMRENDER_H

#include "patern.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <memory>

class RoomRender : public sf::Drawable
{
public:
    RoomRender(std::weak_ptr<Patern> room);
    virtual ~RoomRender() = default;
    void redraw();
    std::weak_ptr<Patern> getRoom() const;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

    void showWallsBoxs(bool value);

    static const unsigned int tileSize;
    static const unsigned int nbTile;

private:
    unsigned int getNbSurfaces() const;
    unsigned int drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const;
    void redrawWalls();

    sf::VertexArray m_render;
    std::weak_ptr<Patern> m_room;
    bool m_showWallsBoxs;
    sf::VertexArray m_wallsRender;
};

#endif // ROOMRENDER_H
