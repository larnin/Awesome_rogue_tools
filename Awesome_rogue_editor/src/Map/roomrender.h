#ifndef ROOMRENDER_H
#define ROOMRENDER_H

#include "patern.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>

class RoomRender : public sf::Drawable
{
public:
    RoomRender(std::weak_ptr<Patern> room, bool showLights);
    RoomRender(RoomRender &&) = default;
    RoomRender & operator=(RoomRender &&) = default;
    virtual ~RoomRender() = default;
    void redraw();
    std::weak_ptr<Patern> getRoom() const;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

    void showWallsBoxs(bool value);

    static const unsigned int tileSize;
    static const unsigned int nbTile;

    inline void setShowLights(bool value) {m_showLights = value;}

private:
    unsigned int getNbSurfaces() const;
    unsigned int drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const;
    void redrawWalls();
    void updateShader() const;
    sf::VertexArray createLightsGismos() const;

    sf::VertexArray m_render;
    std::weak_ptr<Patern> m_room;
    bool m_showWallsBoxs;
    sf::VertexArray m_wallsRender;

    mutable std::unique_ptr<sf::Shader> m_phong;
    bool m_showLights;

    sf::Clock m_clock;
};

#endif // ROOMRENDER_H
