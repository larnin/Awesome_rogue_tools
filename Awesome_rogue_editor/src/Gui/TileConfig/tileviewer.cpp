#include "tileviewer.h"
#include "Utilities/quadrender.h"
#include "Map/blocktype.h"

TileViewer::TileViewer(QWidget *parent)
    : QSFMLCanvas(20, parent)
    , m_box(EMPTY)
    , m_boxOrientation(ROT_0)
    , m_blockID(0)
    , m_tileSize(32)
{

}

void TileViewer::setTexture(const Texture & tex)
{
    m_texture = tex;
}

void TileViewer::setTileSize(unsigned int size)
{
    m_tileSize = size;
}

void TileViewer::setBlockInfo(unsigned int id, BoxType box, Rotation rot)
{
    m_blockID = id;
    m_box = box;
    m_boxOrientation = rot;
}

void TileViewer::OnUpdate()
{
    sf::View v(RenderWindow::getView());
    v.setCenter(0, 0);
    v.setSize(sf::Vector2f(70/64.0f, 70/64.0f));
    RenderWindow::setView(v);

    RenderWindow::clear(sf::Color::Black);

    if(m_texture.isValid())
    {
        sf::FloatRect rect(BlockType::texRect(m_blockID, m_tileSize, m_texture->getSize()));
        sf::VertexArray array(sf::Quads, 4);
        drawQuad(&array[0], sf::FloatRect(-0.5, -0.5, 1, 1), rect);
        RenderWindow::draw(array, sf::RenderStates(m_texture()));
    }

    RenderWindow::draw(toRender(BlockType::createBox(createBoxCaracts(m_boxOrientation, false, false, m_box)), sf::Color::Yellow));
}
