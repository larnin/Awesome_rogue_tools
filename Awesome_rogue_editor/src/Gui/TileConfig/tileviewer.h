#ifndef TILEVIEWER_H
#define TILEVIEWER_H

#include "Gui/qsfmlcanvas.h"
#include "Utilities/ressource.h"
#include "Map/block.h"

class TileViewer : public QSFMLCanvas
{
    Q_OBJECT
public:
    TileViewer(QWidget *parent = nullptr);

    void setTexture(const Texture & tex);
    void setTileSize(unsigned int size);
    void setBlockInfo(unsigned int id, BoxType box, Rotation rot);

protected:
    virtual void OnUpdate();

private:
    BoxType m_box;
    Rotation m_boxOrientation;
    Texture m_texture;
    unsigned int m_blockID;
    unsigned int m_tileSize;
};

#endif // TILEVIEWER_H
