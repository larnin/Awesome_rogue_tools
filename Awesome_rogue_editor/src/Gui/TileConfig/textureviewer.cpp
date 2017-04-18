#include "textureviewer.h"
#include <QMouseEvent>
#include "Utilities/configs.h"
#include <algorithm>
#include "Map/blocktype.h"

TextureViewer::TextureViewer(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_zoomLevel(1)
    , m_okTexture("res/img/check.png")
    , m_tileSize(16)
{
    m_grid.setColor(sf::Color::White);
    m_grid.setDelta(0);
}

void TextureViewer::setTexture(Texture tex)
{
    m_texture = tex;
    if(m_texture.isValid())
        m_grid.setSize(m_texture->getSize()/m_tileSize);
}

void TextureViewer::setTileSize(unsigned int size)
{
    m_tileSize = size;
    if(m_texture.isValid())
        m_grid.setSize(m_texture->getSize()/m_tileSize);
    m_grid.setTileSize(size);
}

void TextureViewer::OnUpdate()
{
    RenderWindow::clear(sf::Color::Black);

    if(!m_texture.isValid())
        return;

    sf::Sprite s(*m_texture);
    RenderWindow::draw(s);
    RenderWindow::draw(m_grid);

    unsigned int maxID(m_texture->getSize().x/m_tileSize*m_texture->getSize().y/m_tileSize);
    for(unsigned int i(0) ; i < maxID ; i++)
    {
        bool isGround(std::find(Configs::tiles.groundIDs.begin(), Configs::tiles.groundIDs.end(), i) != Configs::tiles.groundIDs.end());
        bool isWall(std::find_if(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [i](const auto & v){return v.id == i;}) != Configs::tiles.walls.end());
        if(!isGround && !isWall)
            continue;

        sf::Sprite s(*m_okTexture);
        if(isGround && isWall)
            s.setTextureRect(sf::IntRect(50, 0, 26, 23));
        else if(isGround)
            s.setTextureRect(sf::IntRect(0, 0, 24, 23));
        else s.setTextureRect(sf::IntRect(25, 0, 24, 23));

        sf::FloatRect index(BlockType::texRect(i, m_tileSize, m_texture->getSize()));
        s.setPosition(index.left+(index.width-s.getTextureRect().width)/2, index.top+(index.height-s.getTextureRect().height)/2);

        RenderWindow::draw(s);
    }
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
        sf::Vector2f max(sf::Vector2f(m_texture->getSize()/m_tileSize*m_tileSize));
        if(pos.x > max.x || pos.y > max.y)
            return;

        unsigned int index((unsigned int)(pos.x/m_tileSize)+(unsigned int)(pos.y/m_tileSize)*(m_texture->getSize().x/m_tileSize));
        emit tileSelected(index);
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
