#ifndef CENTRALVIEW_H
#define CENTRALVIEW_H

#include <vector>
#include <memory>

#include "Map/roomrender.h"
#include "qsfmlcanvas.h"
#include "cursorblockrender.h"
#include "selectorblockrender.h"
#include "Utilities/undoinfos.h"

#include "grid.h"

class CentralView : public QSFMLCanvas
{
    Q_OBJECT
public:
    CentralView(QWidget *parent = nullptr);
    virtual ~CentralView() = default;

    static std::vector<float> allZoomLevels();

public slots:
    void setPatern(std::weak_ptr<Patern> patern);
    void setSelectedBlock(const Block & b);
    void setDrawWall(bool value);
    void setDrawGround(bool value);
    void setGridShow(bool value);
    void setGridColor(const sf::Color & color);
    void undo();
    void redo();
    void switchSelectionMode(bool state);
    void copy();
    void past();

    void showWallsHitbox(bool value);
    void onTileConfigChanged();

signals:
    void sendPickedBlock(const Block & b);

protected:
    virtual void showEvent(QShowEvent* event);
    virtual void OnUpdate();
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);
    virtual void resizeEvent(QResizeEvent * event);

private:
    void placeBlock(sf::Vector2f cursorPos);
    void pickBlock(sf::Vector2f cursorPos);
    void onPast();
    std::vector<BlockInfo> executeUndoRedo(const std::vector<BlockInfo> & list);

    float m_zoomLevel;
    RoomRender m_render;
    Grid m_grid;
    bool m_drawGrid;
    bool m_selectMode;
    bool m_showWalls;

    CursorBlockRender m_cursorBlock;
    SelectorBlockRender m_selector;

    UndoInfos m_undoList;
    UndoInfos m_redoList;

};

#endif // CENTRALVIEW_H
