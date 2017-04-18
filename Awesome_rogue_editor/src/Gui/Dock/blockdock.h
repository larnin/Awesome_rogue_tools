#ifndef BLOCKDOCK_H
#define BLOCKDOCK_H

#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>

#include "unclosabledock.h"
#include "blockview.h"
#include "oneblockview.h"
#include "Gui/TileConfig/tileconfigwidget.h"

class BlockDock : public UnclosableDock
{
    Q_OBJECT
public:
    BlockDock(QWidget * parent = nullptr);
    virtual ~BlockDock() = default;

public slots:
    void changeGroundBlockId(unsigned int id);
    void changeWallBlockId(unsigned int id);
    void oneCaracChanged();
    void blockPicked(const Block & b);
    void onTileConfigChanged();

signals:
    void changeBlock(const Block &);
    void tileConfigChanged();

private:
    QWidget* createTabGround();
    QWidget* createTabWall();

    BlockView *m_groundBlocks;
    QCheckBox *m_groundXFlipedWidget;
    QCheckBox *m_groundYFlipedWidget;
    QRadioButton *m_groundRot0Widget;
    QRadioButton *m_groundRot90Widget;
    QRadioButton *m_groundRot180Widget;
    QRadioButton *m_groundRot270Widget;

    BlockView *m_wallBlocks;
    QCheckBox *m_wallXFlipedWidget;
    QCheckBox *m_wallYFlipedWidget;
    QRadioButton *m_wallRot0Widget;
    QRadioButton *m_wallRot90Widget;
    QRadioButton *m_wallRot180Widget;
    QRadioButton *m_wallRot270Widget;

    OneBlockView* m_actualBlock;

    QPushButton *m_tileConfigButton;

    TileConfigWidget m_tileConfigWidget;
};

#endif // BLOCKDOCK_H
