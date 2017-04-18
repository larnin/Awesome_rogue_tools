#ifndef ONEBLOCKVIEW_H
#define ONEBLOCKVIEW_H

#include "Gui/qsfmlcanvas.h"
#include "Utilities/ressource.h"
#include "Map/blocktype.h"

class OneBlockView : public QSFMLCanvas
{
    Q_OBJECT

public:
    OneBlockView(QWidget * parent = nullptr);
    virtual ~OneBlockView() = default;

public slots:
    void changeGroundBlock(unsigned int tileID);
    void changeGroundBlockOrientation(Rotation rot, bool xFliped, bool yFliped);
    void changeWallBlock(unsigned int tileID);
    void changeWallBlockOrientation(Rotation rot, bool xFliped, bool yFliped);

    Block toBlock() const;

protected:
    //virtual void showEvent(QShowEvent* event);
    //virtual void resizeEvent(QResizeEvent * event);
    virtual void OnUpdate();

private:
    unsigned int m_groundTileID;
    Rotation m_groundRot;
    bool m_groundXFliped;
    bool m_groundYFliped;

    unsigned int m_wallTileID;
    Rotation m_wallRot;
    bool m_wallXFliped;
    bool m_wallYFliped;
};

#endif // ONEBLOCKVIEW_H
