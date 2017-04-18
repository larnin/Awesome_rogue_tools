#include "textureviewer.h"
#include "tilesetdata.h"
#include "tile.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <QMouseEvent>

TextureViewer::TextureViewer(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_currentID(0)
    , m_zoomLevel(1)
{
    m_grid.setTileSize(TilesetData::instance.tileSize);
    m_grid.setDelta(0);
}

void TextureViewer::OnUpdate()
{
    RenderWindow::clear(sf::Color::Black);

    Texture & t(TilesetData::instance.texture);
    unsigned int tileSize(TilesetData::instance.tileSize);

    if(!t.isValid())
        return;

    sf::Sprite s(*t);
    RenderWindow::draw(s);
    RenderWindow::draw(m_grid);

    sf::FloatRect index(texRect(m_currentID, tileSize, t->getSize()));
    sf::RectangleShape sharp(sf::Vector2f(index.width, index.height));
    sharp.setFillColor(sf::Color::Transparent);
    sharp.setOutlineColor(sf::Color::Red);
    sharp.setOutlineThickness(2);
    sharp.setPosition(index.left, index.top);
    RenderWindow::draw(sharp);
}

void TextureViewer::onConfigChanged()
{
    m_grid.setTileSize(TilesetData::instance.tileSize);
    m_grid.setDelta(0);
    if(TilesetData::instance.texture.isValid())
        m_grid.setSize(TilesetData::instance.texture->getSize()/TilesetData::instance.tileSize);
}

void TextureViewer::mousePressEvent(QMouseEvent * event)
{
    if(event->buttons() & Qt::MidButton)
        QCursor::setPos(mapToGlobal(QPoint(width()/2.0f, height()/2.0f)));

    if(event->buttons() & Qt::LeftButton)
    {
        sf::Vector2f pos(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
        if(pos.x < 0 || pos.y < 0)
            return;
        Texture &t(TilesetData::instance.texture);
        unsigned int tileSize(TilesetData::instance.tileSize);
        if(!t.isValid())
            return;
        sf::Vector2f max(sf::Vector2f(t->getSize()/tileSize*tileSize));
        if(pos.x > max.x || pos.y > max.y)
            return;

        unsigned int index((unsigned int)(pos.x/tileSize)+(unsigned int)(pos.y/tileSize)*(t->getSize().x/tileSize));
        m_currentID = index;
        emit TileSelected(index);
    }
}

void TextureViewer::mouseMoveEvent(QMouseEvent * event)
{
    if(event->buttons() & Qt::MidButton)
    {
        sf::View v(RenderWindow::getView());
        sf::Vector2f realPos(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
        v.move(v.getCenter() - realPos);
        RenderWindow::setView(v);

        QCursor::setPos(mapToGlobal(QPoint(width()/2.0f, height()/2.0f)));
    }
}

std::vector<float> TextureViewer::allZoomLevels()
{
    std::vector<float> returnTab;
    float value(64);
    int i(9);
    while(i-->0)
        returnTab.push_back(value/=2);
    return returnTab;
}

void TextureViewer::wheelEvent(QWheelEvent * event)
{
    int move = event->angleDelta().y();
    if(move < 0)
        move = -1;
    if(move > 0)
        move = 1;

    const std::vector<float> zooms(allZoomLevels());
    auto it(std::find(zooms.begin(), zooms.end(), m_zoomLevel));
    if(it == zooms.end())
        m_zoomLevel = 1;
    if(it != zooms.begin() && move < 0)
        m_zoomLevel = *(it-1);
    if(it != zooms.end()-1 && move > 0)
        m_zoomLevel = *(it+1);

    sf::View v(RenderWindow::getView());
    v.setSize(width()/m_zoomLevel, height()/m_zoomLevel);
    RenderWindow::setView(v);
}

void TextureViewer::resizeEvent(QResizeEvent * event)
{
    sf::View v(RenderWindow::getView());
    v.setSize(event->size().width()/m_zoomLevel, event->size().height()/m_zoomLevel);
    RenderWindow::setView(v);
}
