#ifndef TEXTUREVIEWER_H
#define TEXTUREVIEWER_H

#include "Gui/qsfmlcanvas.h"
#include "Utilities/ressource.h"
#include "Gui/grid.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

class TextureViewer : public QSFMLCanvas
{
    Q_OBJECT
public:
    TextureViewer(QWidget *parent = nullptr);

    void setTexture(Texture tex);
    void setTileSize(unsigned int size);

signals:
    void tileSelected(unsigned int id);

protected:
    virtual void OnUpdate();
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);
    virtual void resizeEvent(QResizeEvent * event);

private:
    std::vector<float> allZoomLevels();

    float m_zoomLevel;
    Texture m_texture;
    Texture m_okTexture;
    unsigned int m_tileSize;
    unsigned int m_selectedID;
    Grid m_grid;
    sf::Clock m_clock;
};

#endif // TEXTUREVIEWER_H
