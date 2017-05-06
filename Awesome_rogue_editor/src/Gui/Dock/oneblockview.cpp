#include "oneblockview.h"
#include "Utilities/quadrender.h"
#include "Utilities/configs.h"
#include <SFML/Graphics/VertexArray.hpp>

OneBlockView::OneBlockView(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_groundTileID(0)
    , m_groundRot(Rotation::ROT_0)
    , m_groundXFliped(false)
    , m_groundYFliped(false)
    , m_wallTileID(0)
    , m_wallRot(Rotation::ROT_0)
    , m_wallXFliped(false)
    , m_wallYFliped(false)
    , m_topTileID(0)
    , m_topRot(Rotation::ROT_0)
    , m_topXFliped(false)
    , m_topYFliped(false)
{

}

void OneBlockView::changeGroundBlock(unsigned int tileID)
{
    m_groundTileID = tileID;
}

void OneBlockView::changeGroundBlockOrientation(Rotation rot, bool xFliped, bool yFliped)
{
    m_groundRot = rot;
    m_groundXFliped = xFliped;
    m_groundYFliped = yFliped;
}

void OneBlockView::changeWallBlock(unsigned int tileID)
{
    m_wallTileID = tileID;
}

void OneBlockView::changeWallBlockOrientation(Rotation rot, bool xFliped, bool yFliped)
{
    m_wallRot = rot;
    m_wallXFliped = xFliped;
    m_wallYFliped = yFliped;
}


void OneBlockView::changeTopBlock(unsigned int tileID)
{
    m_topTileID = tileID;
}

void OneBlockView::changeTopBlockOrientation(Rotation rot, bool xFliped, bool yFLiped)
{
    m_topRot = rot;
    m_topXFliped = xFliped;
    m_topYFliped = yFLiped;
}

/*void OneBlockView::showEvent(QShowEvent* event)
{
    QSFMLCanvas::showEvent(event);

    sf::View v(RenderWindow::getView());
    v.setCenter(sf::Vector2f(BlockType::blockSize, BlockType::blockSize)/2.0f);
    v.zoom(0.25f);
    RenderWindow::setView(v);
}

void OneBlockView::resizeEvent(QResizeEvent * event)
{
    QSFMLCanvas::resizeEvent(event);

    sf::View v(RenderWindow::getView());
    v.setCenter(sf::Vector2f(BlockType::blockSize, BlockType::blockSize)/2.0f);
    v.zoom(0.25f);
    RenderWindow::setView(v);
}*/

void OneBlockView::OnUpdate()
{
    sf::View v(RenderWindow::getView());
    v.setCenter(0, 0);
    v.setSize(sf::Vector2f(70/64.0f, 70/64.0f));
    RenderWindow::setView(v);

    RenderWindow::clear(sf::Color::White);

    if(Configs::tiles.texture.isValid())
    {
        unsigned int tile(Configs::tiles.tileSize);
        sf::FloatRect groundRect(BlockType::texRect(m_groundTileID, tile, Configs::tiles.texture->getSize()));
        sf::FloatRect wallRect(BlockType::texRect(m_wallTileID, tile, Configs::tiles.texture->getSize()));
        sf::FloatRect topRect(BlockType::texRect(m_topTileID, tile, Configs::tiles.texture->getSize()));

        sf::VertexArray array(sf::Quads, 12);
        drawQuad(&array[0], sf::FloatRect(-0.5, -0.5, 1, 1), groundRect, m_groundXFliped, m_groundYFliped, m_groundRot);
        drawQuad(&array[4], sf::FloatRect(-0.5, -0.5, 1, 1), wallRect, m_wallXFliped, m_wallYFliped, m_wallRot);
        drawQuad(&array[8], sf::FloatRect(-0.5, -0.5, 1, 1), topRect, m_topXFliped, m_topYFliped, m_topRot);
        RenderWindow::draw(array, sf::RenderStates(Configs::tiles.texture()));
    }
}

Block OneBlockView::toBlock() const
{
    return Block(m_groundTileID, createOrientation(m_groundRot, m_groundXFliped, m_groundYFliped)
                 , m_wallTileID, createOrientation(m_wallRot, m_wallXFliped, m_wallYFliped)
                 , transformData(BlockType::baseBoxCaractOf(m_wallTileID), m_wallRot, m_wallXFliped, m_wallYFliped)
                 , m_topTileID, createOrientation(m_topRot, m_topXFliped, m_topYFliped));
}
