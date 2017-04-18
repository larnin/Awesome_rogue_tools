#include <QWheelEvent>
#include <QMouseEvent>

#include "blockview.h"
#include "Utilities/quadrender.h"
#include <iostream>
#include "Utilities/configs.h"

BlockView::BlockView(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_array(sf::Quads)
    , m_selected(false)
    , m_selectedId(0)
    , m_type(TypeTile::TILE_WALL)
{
    setMouseTracking(true);

    drawTiles();
}

void BlockView::OnUpdate()
{
    RenderWindow::clear(sf::Color::White);
    if(Configs::tiles.texture.isValid())
        RenderWindow::draw(m_array, sf::RenderStates(Configs::tiles.texture()));
    checkSelected();
    if(m_selected)
        RenderWindow::draw(generactSelector());
}

void BlockView::changeView(TypeTile type)
{
    m_type = type;
    drawTiles();
}

void BlockView::showEvent(QShowEvent* event)
{
    QSFMLCanvas::showEvent(event);

    sf::View v(RenderWindow::getView());
    v.setSize(sf::Vector2f(width()/2.0f, height()/2.0f));
    v.setCenter(v.getSize()/2.0f);
    RenderWindow::setView(v);
}

void BlockView::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() != Qt::LeftButton)
        return;

    sf::Vector2i realPos(sf::Vector2i(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())))/int(Configs::tiles.tileSize+2));
    int index(realPos.x + realPos.y*(width()/2/(Configs::tiles.tileSize+2)));
    std::vector<unsigned int> ids(BlockType::getID(m_type));
    if(index >= int(ids.size()) || index < 0)
        return;
    emit selectBlock(ids[index]);
    m_selectedId = ids[index];
    m_selected = true;
}

void BlockView::wheelEvent(QWheelEvent * event)
{
    sf::View v(RenderWindow::getView());
    if(v.getSize().y >= m_botomTiles)
        return;

    int move = event->angleDelta().y();
    if(move < 0)
        move = -20;
    if(move > 0)
        move = 20;
    move = -move;

    if (v.getCenter().y + move - v.getSize().y/2 < 0)
        v.setCenter(v.getSize()/2.0f);
    else if(v.getCenter().y + move + v.getSize().y/2>m_botomTiles)
        v.setCenter(v.getSize().x/2, m_botomTiles-v.getSize().y/2);
    else v.setCenter(v.getCenter().x, v.getCenter().y+move);
    RenderWindow::setView(v);
}

void BlockView::resizeEvent(QResizeEvent * event)
{
    QSFMLCanvas::resizeEvent(event);
    drawTiles();
    sf::View v(RenderWindow::getView());
    v.setSize(sf::Vector2f(width()/2.0f, height()/2.0f));
    v.setCenter(v.getSize()/2.0f);
    RenderWindow::setView(v);
}

void BlockView::drawTiles()
{
    m_array.clear();
    std::vector<unsigned int> ids(BlockType::getID(m_type));
    m_array.resize(ids.size()*4);

    const unsigned int caseWidth(Configs::tiles.tileSize+2);
    unsigned int nbWidth(width() / 2 / caseWidth);
    if(nbWidth == 0)
        return;

    for(unsigned int i(0) ; i < ids.size() ; i++)
    {
        unsigned int id(ids[i]);
        sf::Vector2f pos(i%nbWidth*caseWidth+1, i/nbWidth*caseWidth+1);
        sf::FloatRect tex(BlockType::texRect(id, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
        drawQuad(&(m_array[i*4]), sf::FloatRect(pos, sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)), tex);
    }

    m_botomTiles = ids.size()/nbWidth*caseWidth+1+caseWidth;
}

void BlockView::setBlock(unsigned int id)
{
    m_selectedId = id;
    m_selected = true;
}

sf::RectangleShape BlockView::generactSelector() const
{
    sf::RectangleShape sharp;
    sharp.setSize(sf::Vector2f(Configs::tiles.tileSize,Configs::tiles.tileSize));
    sharp.setOutlineColor(sf::Color::Red);
    sharp.setFillColor(sf::Color::Transparent);
    sharp.setOutlineThickness(2);

    std::vector<unsigned int> ids(BlockType::getID(m_type));
    auto it(std::find(ids.begin(), ids.end(), m_selectedId));
    if(it == ids.end())
    {
        sharp.setPosition(sf::Vector2f(-2*Configs::tiles.tileSize, -2*Configs::tiles.tileSize));
        return sharp;
    }
    unsigned int index(it-ids.begin());
    const unsigned int caseWidth(Configs::tiles.tileSize+2);
    unsigned int nbWidth(width() / 2 / caseWidth);
    sharp.setPosition(index % nbWidth * caseWidth + 1, index / nbWidth * caseWidth + 1);

    return sharp;
}

void BlockView::checkSelected()
{
    std::vector<unsigned int> ids(BlockType::getID(m_type));
    auto it(std::find(ids.begin(), ids.end(), m_selectedId));
    if(it == ids.end())
    {
        m_selected = false;
        return;
    }
}
