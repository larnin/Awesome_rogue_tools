#ifndef BLOCKDOCK_H
#define BLOCKDOCK_H

#include "unclosabledock.h"
#include "blockview.h"
#include "oneblockview.h"
#include "Gui/TileConfig/tileconfigwidget.h"
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>

class BlockDock : public UnclosableDock
{
    Q_OBJECT
public:
    BlockDock(QWidget * parent = nullptr);
    virtual ~BlockDock() = default;

public slots:
    void changeGroundBlockId(unsigned int id);
    void changeWallBlockId(unsigned int id);
    void changeTopBlockId(unsigned int id);
    void oneCaracChanged();
    void blockPicked(const Block & b);
    void onTileConfigChanged();

signals:
    void changeBlock(const Block &);
    void tileConfigChanged();

private:
    QWidget* createTabGround();
    QWidget* createTabWall();
    QWidget* createTabTop();

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

    BlockView *m_topBlocks;
    QCheckBox *m_topXFlipedWidget;
    QCheckBox *m_topYFlipedWidget;
    QRadioButton *m_topRot0Widget;
    QRadioButton *m_topRot90Widget;
    QRadioButton *m_topRot180Widget;
    QRadioButton *m_topRot270Widget;

    OneBlockView* m_actualBlock;

    QPushButton *m_tileConfigButton;

    TileConfigWidget m_tileConfigWidget;
};

#endif // BLOCKDOCK_H
