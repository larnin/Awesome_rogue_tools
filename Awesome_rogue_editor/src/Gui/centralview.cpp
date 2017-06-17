#include "Utilities/configs.h"
#include "centralview.h"
#include "Map/blocktype.h"
#include <SFML/Window/Mouse.hpp>
#include <QWheelEvent>
#include <QCursor>
#include <algorithm>

CentralView::CentralView(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_zoomLevel(1)
    , m_render(std::weak_ptr<Patern>())
    , m_drawGrid(false)
    , m_selectMode(false)
    , m_showWalls(false)
    , m_showLights(false)
    , m_ambiant(1.0f)
{
    m_grid.setTileSize(Configs::tiles.tileSize);
    m_grid.setDelta(Configs::tiles.tileSize/2.0f);
}

void CentralView::showEvent(QShowEvent* event)
{
    QSFMLCanvas::showEvent(event);

    std::shared_ptr<Patern> pLock(m_render.getRoom().lock());
    if(pLock)
    {
        sf::View v(RenderWindow::getView());
        v.setCenter(sf::Vector2f(pLock->getSize()*Configs::tiles.tileSize)/2.0f);
        v.setSize(width()/m_zoomLevel, height()/m_zoomLevel);
        RenderWindow::setView(v);
    }
}

void CentralView::OnUpdate()
{
    RenderWindow::clear(sf::Color::Black);
    RenderWindow::draw(m_render);
    if(m_drawGrid)
        RenderWindow::draw(m_grid);

    if(m_selectMode)
    {
        m_selector.setMousePos(sf::Mouse::getPosition(*this));
        RenderWindow::draw(m_selector);
    }
    else
    {
        m_cursorBlock.setMousePos(sf::Mouse::getPosition(*this));
        RenderWindow::draw(m_cursorBlock);
    }
}

void CentralView::mousePressEvent(QMouseEvent * event)
{
    if(event->buttons() & Qt::MidButton)
        QCursor::setPos(mapToGlobal(QPoint(width()/2.0f, height()/2.0f)));

    if(event->buttons() & Qt::LeftButton)
    {
        if(m_selectMode)
        {
            if(m_selector.inPast)
                onPast();
            else
            {
                sf::Vector2f cursorPos(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
                cursorPos += sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f;
                cursorPos /= float(Configs::tiles.tileSize);
                m_selector.setPos1(sf::Vector2u(cursorPos));
            }
        }
        else placeBlock(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
    }

    if(event->buttons() & Qt::RightButton)
        pickBlock(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
}

void CentralView::mouseMoveEvent(QMouseEvent * event)
{
    if(event->buttons() & Qt::MidButton)
    {
        sf::View v(RenderWindow::getView());
        sf::Vector2f realPos(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
        v.move(v.getCenter() - realPos);
        RenderWindow::setView(v);

        QCursor::setPos(mapToGlobal(QPoint(width()/2.0f, height()/2.0f)));
    }
    if(event->buttons() & Qt::LeftButton)
    {
        if(m_selectMode)
        {
            if(!m_selector.inPast)
            {
                sf::Vector2f cursorPos(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
                cursorPos += sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f;
                cursorPos /= float(Configs::tiles.tileSize);
                if(cursorPos.x > 0 && cursorPos.y > 0)
                    m_selector.setPos2(sf::Vector2u(cursorPos));
            }
        }
        else placeBlock(RenderWindow::mapPixelToCoords(sf::Vector2i(event->localPos().x(), event->localPos().y())));
    }
}

void CentralView::wheelEvent(QWheelEvent * event)
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

void CentralView::resizeEvent(QResizeEvent * event)
{
    sf::View v(RenderWindow::getView());
    v.setSize(event->size().width()/m_zoomLevel, event->size().height()/m_zoomLevel);
    RenderWindow::setView(v);
}

void CentralView::setPatern(std::weak_ptr<Patern> patern)
{
    m_render = RoomRender(patern);
    m_render.showWallsBoxs(m_showWalls);

    m_zoomLevel = 1;
    sf::View v(RenderWindow::getView());
    std::shared_ptr<Patern> pLock(patern.lock());
    if(pLock)
    {
        v.setCenter(sf::Vector2f(pLock->getSize()*Configs::tiles.tileSize)/2.0f);
        m_grid.setSize(pLock->getSize());
    }
    else m_grid.setSize(sf::Vector2u(0, 0));
    v.setSize(width()/m_zoomLevel, height()/m_zoomLevel);
    RenderWindow::setView(v);

    m_undoList.clear();
    m_redoList.clear();
}

void CentralView::setSelectedBlock(const Block & b)
{
    m_cursorBlock.block = b;
}

void CentralView::setDrawWall(bool value)
{
    m_cursorBlock.drawWall = value;
}

void CentralView::setDrawGround(bool value)
{
    m_cursorBlock.drawGround = value;
}

void CentralView::setDrawTop(bool value)
{
    m_cursorBlock.drawTop = value;
}

void CentralView::setAmbiant(float value)
{
    m_ambiant = value;
}

void CentralView::placeBlock(sf::Vector2f cursorPos)
{
    std::shared_ptr<Patern> pLock(m_render.getRoom().lock());
    if(!pLock)
        return;

    cursorPos += sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f;
    cursorPos /= float(Configs::tiles.tileSize);
    sf::Vector2i blockPos(cursorPos);
    if(blockPos.x < 0 || blockPos.y < 0 || blockPos.x >= int(pLock->getSize().x)  || blockPos.y >= int(pLock->getSize().y))
        return;

    m_redoList.clear();
    Block oldBlock((*pLock)(sf::Vector2u(blockPos)));
    bool toAdd(false);

    if(m_cursorBlock.drawGround)
    {
        if((*pLock)(sf::Vector2u(blockPos)).groundID != m_cursorBlock.block.groundID
                || (*pLock)(sf::Vector2u(blockPos)).groundOrientation != m_cursorBlock.block.groundOrientation)
        {
            toAdd = true;
            (*pLock)(sf::Vector2u(blockPos)).groundID = m_cursorBlock.block.groundID;
            (*pLock)(sf::Vector2u(blockPos)).groundOrientation = m_cursorBlock.block.groundOrientation;
        }
    }
    if(m_cursorBlock.drawTop)
    {
        if((*pLock)(sf::Vector2u(blockPos)).topID != m_cursorBlock.block.topID
                || (*pLock)(sf::Vector2u(blockPos)).topOrientation != m_cursorBlock.block.topOrientation)
        {
            toAdd = true;
            (*pLock)(sf::Vector2u(blockPos)).topID = m_cursorBlock.block.topID;
            (*pLock)(sf::Vector2u(blockPos)).topOrientation = m_cursorBlock.block.topOrientation;
        }
    }
    if(m_cursorBlock.drawWall)
    {
        if((*pLock)(sf::Vector2u(blockPos)).wallID != m_cursorBlock.block.wallID
                || (*pLock)(sf::Vector2u(blockPos)).wallOrientation != m_cursorBlock.block.wallOrientation
                || (*pLock)(sf::Vector2u(blockPos)).boxCaracts != m_cursorBlock.block.boxCaracts)
        {
            toAdd = true;
            (*pLock)(sf::Vector2u(blockPos)).wallID = m_cursorBlock.block.wallID;
            (*pLock)(sf::Vector2u(blockPos)).wallOrientation = m_cursorBlock.block.wallOrientation;
            (*pLock)(sf::Vector2u(blockPos)).boxCaracts = m_cursorBlock.block.boxCaracts;
        }
    }

    if(toAdd)
        m_undoList.addPoint(BlockInfo(sf::Vector2u(blockPos), oldBlock));

    m_render.redraw();
}

void CentralView::pickBlock(sf::Vector2f cursorPos)
{
    std::shared_ptr<Patern> pLock(m_render.getRoom().lock());
    if(!pLock)
        return;

    cursorPos += sf::Vector2f(Configs::tiles.tileSize, Configs::tiles.tileSize)/2.0f;
    cursorPos /= float(Configs::tiles.tileSize);
    sf::Vector2i blockPos(cursorPos);
    if(blockPos.x < 0 || blockPos.y < 0 || blockPos.x >= int(pLock->getSize().x)  || blockPos.y >= int(pLock->getSize().y))
        return;

    if(m_cursorBlock.drawGround)
    {
        m_cursorBlock.block.groundID = (*pLock)(sf::Vector2u(blockPos)).groundID;
        m_cursorBlock.block.groundOrientation = (*pLock)(sf::Vector2u(blockPos)).groundOrientation;
    }
    if(m_cursorBlock.drawTop)
    {
        m_cursorBlock.block.topID = (*pLock)(sf::Vector2u(blockPos)).topID;
        m_cursorBlock.block.topOrientation = (*pLock)(sf::Vector2u(blockPos)).topOrientation;
    }
    if(m_cursorBlock.drawWall)
    {
        m_cursorBlock.block.wallID = (*pLock)(sf::Vector2u(blockPos)).wallID;
        m_cursorBlock.block.wallOrientation = (*pLock)(sf::Vector2u(blockPos)).wallOrientation;
        m_cursorBlock.block.boxCaracts = (*pLock)(sf::Vector2u(blockPos)).boxCaracts;
    }

    emit sendPickedBlock(m_cursorBlock.block);
}


void CentralView::setGridShow(bool value)
{
    m_drawGrid = value;
}

void CentralView::setGridColor(const sf::Color & color)
{
    m_grid.setColor(color);
}

void CentralView::setDrawLights(bool value)
{
    m_showLights = value;
}

void CentralView::undo()
{
    auto point(m_undoList.takeLast());
    if(point.empty())
        return;

    m_redoList.addPoint(executeUndoRedo(point));
}

void CentralView::redo()
{
    auto point(m_redoList.takeLast());
    if(point.empty())
        return;

    m_undoList.addPoint(executeUndoRedo(point));
}

std::vector<BlockInfo> CentralView::executeUndoRedo(const std::vector<BlockInfo> & list)
{
    std::shared_ptr<Patern> pLock(m_render.getRoom().lock());
    if(!pLock)
        return std::vector<BlockInfo>();

    std::vector<BlockInfo> oldBlocks;

    for(const BlockInfo & b : list)
    {
        if(pLock->getSize().x <= b.pos.x || pLock->getSize().y <= b.pos.y)
            continue;
        oldBlocks.push_back(BlockInfo(b.pos, (*pLock)(b.pos)));
        (*pLock)(b.pos) = b.block;
    }
    m_render.redraw();
    return oldBlocks;
}


void CentralView::copy()
{
    if(!m_selectMode)
        return;
    if(m_selector.inPast)
        return;

    std::shared_ptr<Patern> pLock(m_render.getRoom().lock());
    if(!pLock)
        return;

    sf::IntRect rect(m_selector.getSelectedZone());
    if(rect.left + rect.width > int(pLock->getSize().x))
        rect.width = pLock->getSize().x - rect.left;
    if(rect.top + rect.height > int(pLock->getSize().y))
        rect.height = pLock->getSize().y - rect.top;
    if(rect.left < 0)
    {
        rect.width-= rect.left;
        rect.left = 0;
    }
    if(rect.top < 0)
    {
        rect.height -= rect.top;
        rect.top = 0;
    }

    if(rect.width <= 0 || rect.height <= 0)
        return;

    m_selector.copyBuffer = Matrix<Block>(sf::Vector2u(rect.width, rect.height));
    for(unsigned int i(0) ; i < (unsigned int)(rect.width) ; i++)
        for(unsigned int j(0) ; j < (unsigned int)(rect.height) ; j++)
            m_selector.copyBuffer(sf::Vector2u(i, j)) = (*pLock)(sf::Vector2u(i+rect.left, j+rect.top));
}

void CentralView::past()
{
    if(!m_selectMode)
        return;
    if(m_selector.copyBuffer.getSize() == sf::Vector2u(0, 0))
        return;
    m_selector.inPast = true;
}

void CentralView::onPast()
{
    m_selector.clear();
    m_selector.inPast = false;

    sf::Vector2i mousePos(sf::Vector2i(RenderWindow::mapPixelToCoords(sf::Mouse::getPosition(*this)) / float(Configs::tiles.tileSize) + sf::Vector2f(3, 3)/2.0f));
    sf::Vector2i topLeftPos(mousePos - sf::Vector2i(m_selector.copyBuffer.getSize())/2);

    std::shared_ptr<Patern> pLock(m_render.getRoom().lock());
    if(!pLock)
        return;

    std::vector<BlockInfo> undoList;

    for(unsigned int i(0) ; i < m_selector.copyBuffer.getSize().x ; i++)
        for(unsigned int j(0) ; j < m_selector.copyBuffer.getSize().y ; j++)
        {
            sf::Vector2i pos(topLeftPos + sf::Vector2i(i, j));
            if(pos.x < 0 || pos.y < 0 || pos.x >= int(pLock->getSize().x) || pos.y >= int(pLock->getSize().y))
                continue;
            undoList.push_back(BlockInfo(sf::Vector2u(pos), (*pLock)(sf::Vector2u(pos))));
            (*pLock)(sf::Vector2u(pos)) = m_selector.copyBuffer(sf::Vector2u(i, j));
        }

    if(!undoList.empty())
    {
        m_redoList.clear();
        m_undoList.addPoint(undoList);
    }

    m_render.redraw();
}

void CentralView::switchSelectionMode(bool state)
{
    m_selectMode = state;
    if(state)
        m_selector.clear();
}

std::vector<float> CentralView::allZoomLevels()
{
    std::vector<float> returnTab;
    float value(64);
    int i(9);
    while(i-->0)
        returnTab.push_back(value/=2);
    return returnTab;
}

void CentralView::showWallsHitbox(bool value)
{
    m_render.showWallsBoxs(value);
    m_showWalls = value;
}

void CentralView::onTileConfigChanged()
{
    std::shared_ptr<Patern> pLock(m_render.getRoom().lock());
    if(pLock)
    {
        sf::View v(RenderWindow::getView());
        v.setCenter(sf::Vector2f(pLock->getSize()*Configs::tiles.tileSize)/2.0f);
        v.setSize(width()/m_zoomLevel, height()/m_zoomLevel);
        RenderWindow::setView(v);
    }

    m_grid.setTileSize(Configs::tiles.tileSize);
    m_grid.setDelta(Configs::tiles.tileSize/2.0f);
    m_render.redraw();
}

