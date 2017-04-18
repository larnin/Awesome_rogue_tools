#ifndef TEXTUREVIEWER_H
#define TEXTUREVIEWER_H

#include "qsfmlcanvas.h"
#include "grid.h"

class TextureViewer : public QSFMLCanvas
{
    Q_OBJECT

public:
    TextureViewer(QWidget *parent = nullptr);
    virtual ~TextureViewer() = default;

signals:
    void TileSelected(unsigned int id);

public slots:
    void onConfigChanged();

protected:
    virtual void OnUpdate();
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);
    virtual void resizeEvent(QResizeEvent * event);

private:
    std::vector<float> allZoomLevels();

    unsigned int m_currentID;
    Grid m_grid;
    float m_zoomLevel;
};

#endif // TEXTUREVIEWER_H
