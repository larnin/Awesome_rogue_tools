#include "blockdock.h"
#include <QFrame>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>

BlockDock::BlockDock(QWidget * parent)
    : UnclosableDock("Blocs", parent)
{
    m_tileConfigButton = new QPushButton("Configurer les blocs");

    QTabWidget *tabsWidget = new QTabWidget();
    tabsWidget->addTab(createTabGround(), "Sols");
    tabsWidget->addTab(createTabWall(), "Murs");
    tabsWidget->addTab(createTabTop(), "Plafond");

    m_actualBlock = new OneBlockView();
    m_actualBlock->setFixedSize(4*(16+2), 4*(16+2));
    QFrame *frameBlock = new QFrame();
    frameBlock->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *frameBlockLayout = new QVBoxLayout();
    frameBlockLayout->addWidget(m_actualBlock);
    frameBlock->setLayout(frameBlockLayout);

    QVBoxLayout *layoutBlocks = new QVBoxLayout();
    layoutBlocks->addWidget(m_tileConfigButton);
    layoutBlocks->addWidget(new QLabel("Blocs :"));
    layoutBlocks->addWidget(tabsWidget, 1);
    layoutBlocks->addWidget(new QLabel("Aperçu :"));
    layoutBlocks->addWidget(frameBlock, 0, Qt::AlignCenter);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layoutBlocks);
    setWidget(centralWidget);

    connect(m_tileConfigButton, SIGNAL(clicked()), &m_tileConfigWidget, SLOT(show()));

    connect(m_groundBlocks, SIGNAL(selectBlock(uint)), this, SLOT(changeGroundBlockId(uint)));
    connect(m_wallBlocks, SIGNAL(selectBlock(uint)), this, SLOT(changeWallBlockId(uint)));
    connect(m_topBlocks, SIGNAL(selectBlock(uint)), this, SLOT(changeTopBlockId(uint)));

    connect(m_groundRot0Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_groundRot90Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_groundRot180Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_groundRot270Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_groundXFlipedWidget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_groundYFlipedWidget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));

    connect(m_wallRot0Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_wallRot90Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_wallRot180Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_wallRot270Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_wallXFlipedWidget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_wallYFlipedWidget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));

    connect(m_topRot0Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_topRot90Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_topRot180Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_topRot270Widget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_topXFlipedWidget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));
    connect(m_topYFlipedWidget, SIGNAL(clicked()), this, SLOT(oneCaracChanged()));

    connect(&m_tileConfigWidget, SIGNAL(tileConfigChanged()), this, SLOT(onTileConfigChanged()));
}

QWidget* BlockDock::createTabGround()
{
    m_groundBlocks = new BlockView();
    m_groundBlocks->setMinimumHeight(2*(16+2));
    m_groundBlocks->changeView(TypeTile::TILE_GROUND);
    QFrame *frameBlocks = new QFrame();
    frameBlocks->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *frameBlocksLayout = new QVBoxLayout();
    frameBlocksLayout->addWidget(m_groundBlocks);
    frameBlocks->setLayout(frameBlocksLayout);

    m_groundRot0Widget = new QRadioButton("0°");
    m_groundRot0Widget->setChecked(true);
    m_groundRot90Widget = new QRadioButton("90°");
    m_groundRot180Widget = new QRadioButton("180°");
    m_groundRot270Widget = new QRadioButton("270°");
    QHBoxLayout *rotLayout = new QHBoxLayout();
    rotLayout->addWidget(m_groundRot0Widget);
    rotLayout->addWidget(m_groundRot90Widget);
    rotLayout->addWidget(m_groundRot180Widget);
    rotLayout->addWidget(m_groundRot270Widget);
    QGroupBox *rotBox = new QGroupBox("Rotation :");
    rotBox->setLayout(rotLayout);

    m_groundXFlipedWidget = new QCheckBox(" Horisontal");
    m_groundYFlipedWidget = new QCheckBox(" Vertical");
    QHBoxLayout *flipLayout = new QHBoxLayout();
    flipLayout->addWidget(m_groundXFlipedWidget);
    flipLayout->addWidget(m_groundYFlipedWidget);
    QGroupBox *flipBox = new QGroupBox("Flip :");
    flipBox->setLayout(flipLayout);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(frameBlocks, 1);
    layout->addSpacing(5);
    layout->addWidget(rotBox);
    layout->addWidget(flipBox);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

QWidget* BlockDock::createTabWall()
{
    m_wallBlocks = new BlockView();
    m_wallBlocks->setMinimumHeight(2*(16+2));
    m_wallBlocks->changeView(TypeTile::TILE_WALL);
    QFrame *frameBlocks = new QFrame();
    frameBlocks->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *frameBlocksLayout = new QVBoxLayout();
    frameBlocksLayout->addWidget(m_wallBlocks);
    frameBlocks->setLayout(frameBlocksLayout);

    m_wallRot0Widget = new QRadioButton("0°");
    m_wallRot0Widget->setChecked(true);
    m_wallRot90Widget = new QRadioButton("90°");
    m_wallRot180Widget = new QRadioButton("180°");
    m_wallRot270Widget = new QRadioButton("270°");
    QHBoxLayout *rotLayout = new QHBoxLayout();
    rotLayout->addWidget(m_wallRot0Widget);
    rotLayout->addWidget(m_wallRot90Widget);
    rotLayout->addWidget(m_wallRot180Widget);
    rotLayout->addWidget(m_wallRot270Widget);
    QGroupBox *rotBox = new QGroupBox("Rotation :");
    rotBox->setLayout(rotLayout);

    m_wallXFlipedWidget = new QCheckBox(" Horisontal");
    m_wallYFlipedWidget = new QCheckBox(" Vertical");
    QHBoxLayout *flipLayout = new QHBoxLayout();
    flipLayout->addWidget(m_wallXFlipedWidget);
    flipLayout->addWidget(m_wallYFlipedWidget);
    QGroupBox *flipBox = new QGroupBox("Flip :");
    flipBox->setLayout(flipLayout);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(frameBlocks, 1);
    layout->addSpacing(5);
    layout->addWidget(rotBox);
    layout->addWidget(flipBox);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

QWidget* BlockDock::createTabTop()
{
    m_topBlocks = new BlockView();
    m_topBlocks->setMinimumHeight(2*(16+2));
    m_topBlocks->changeView(TypeTile::TILE_TOP);
    QFrame *frameBlocks = new QFrame();
    frameBlocks->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *frameBlocksLayout = new QVBoxLayout();
    frameBlocksLayout->addWidget(m_topBlocks);
    frameBlocks->setLayout(frameBlocksLayout);

    m_topRot0Widget = new QRadioButton("0°");
    m_topRot0Widget->setChecked(true);
    m_topRot90Widget = new QRadioButton("90°");
    m_topRot180Widget = new QRadioButton("180°");
    m_topRot270Widget = new QRadioButton("270°");
    QHBoxLayout *rotLayout = new QHBoxLayout();
    rotLayout->addWidget(m_topRot0Widget);
    rotLayout->addWidget(m_topRot90Widget);
    rotLayout->addWidget(m_topRot180Widget);
    rotLayout->addWidget(m_topRot270Widget);
    QGroupBox *rotBox = new QGroupBox("Rotation :");
    rotBox->setLayout(rotLayout);

    m_topXFlipedWidget = new QCheckBox(" Horisontal");
    m_topYFlipedWidget = new QCheckBox(" Vertical");
    QHBoxLayout *flipLayout = new QHBoxLayout();
    flipLayout->addWidget(m_topXFlipedWidget);
    flipLayout->addWidget(m_topYFlipedWidget);
    QGroupBox *flipBox = new QGroupBox("Flip :");
    flipBox->setLayout(flipLayout);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(frameBlocks, 1);
    layout->addSpacing(5);
    layout->addWidget(rotBox);
    layout->addWidget(flipBox);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

void BlockDock::changeGroundBlockId(unsigned int id)
{
    m_actualBlock->changeGroundBlock(id);
    emit changeBlock(m_actualBlock->toBlock());
}

void BlockDock::changeWallBlockId(unsigned int id)
{
    m_actualBlock->changeWallBlock(id);
    emit changeBlock(m_actualBlock->toBlock());
}

void BlockDock::changeTopBlockId(unsigned int id)
{
    m_actualBlock->changeTopBlock(id);
    emit changeBlock(m_actualBlock->toBlock());
}

void BlockDock::oneCaracChanged()
{
    Rotation groundRot;
    if(m_groundRot0Widget->isChecked())
        groundRot = Rotation::ROT_0;
    else if (m_groundRot90Widget->isChecked())
        groundRot = Rotation::ROT_90;
    else if (m_groundRot180Widget->isChecked())
        groundRot = Rotation::ROT_180;
    else groundRot = Rotation::ROT_270;
    m_actualBlock->changeGroundBlockOrientation(groundRot, m_groundXFlipedWidget->isChecked(), m_groundYFlipedWidget->isChecked());

    Rotation wallRot;
    if(m_wallRot0Widget->isChecked())
        wallRot = Rotation::ROT_0;
    else if (m_wallRot90Widget->isChecked())
        wallRot = Rotation::ROT_90;
    else if (m_wallRot180Widget->isChecked())
        wallRot = Rotation::ROT_180;
    else wallRot = Rotation::ROT_270;
    m_actualBlock->changeWallBlockOrientation(wallRot, m_wallXFlipedWidget->isChecked(), m_wallYFlipedWidget->isChecked());

    Rotation topRot;
    if(m_topRot0Widget->isChecked())
        topRot = Rotation::ROT_0;
    else if (m_topRot90Widget->isChecked())
        topRot = Rotation::ROT_90;
    else if (m_topRot180Widget->isChecked())
        topRot = Rotation::ROT_180;
    else topRot = Rotation::ROT_270;
    m_actualBlock->changeWallBlockOrientation(topRot, m_topXFlipedWidget->isChecked(), m_topYFlipedWidget->isChecked());

    emit changeBlock(m_actualBlock->toBlock());
}


void BlockDock::blockPicked(const Block & b)
{
    m_groundBlocks->setBlock(b.groundID);
    Rotation groundRot(getRotation(b.groundOrientation));
    if(groundRot == Rotation::ROT_0)
        m_groundRot0Widget->setChecked(true);
    else if(groundRot == Rotation::ROT_90)
        m_groundRot90Widget->setChecked(true);
    else if(groundRot == ROT_180)
        m_groundRot180Widget->setChecked(true);
    else m_groundRot270Widget->setChecked(true);
    bool groundXFliped(getXFlip(b.groundOrientation));
    bool groundYFliped(getYFlip(b.groundOrientation));
    m_groundXFlipedWidget->setChecked(groundXFliped);
    m_groundYFlipedWidget->setChecked(groundYFliped);

    m_actualBlock->changeGroundBlock(b.groundID);
    m_actualBlock->changeGroundBlockOrientation(groundRot, groundXFliped, groundYFliped);

    m_wallBlocks->setBlock(b.wallID);
    Rotation wallRot(getRotation(b.wallOrientation));
    if(wallRot == Rotation::ROT_0)
        m_wallRot0Widget->setChecked(true);
    else if(wallRot == Rotation::ROT_90)
        m_wallRot90Widget->setChecked(true);
    else if(wallRot == ROT_180)
        m_wallRot180Widget->setChecked(true);
    else m_wallRot270Widget->setChecked(true);
    bool wallXFliped(getXFlip(b.wallOrientation));
    bool wallYFliped(getYFlip(b.wallOrientation));
    m_wallXFlipedWidget->setChecked(wallXFliped);
    m_wallYFlipedWidget->setChecked(wallYFliped);

    m_actualBlock->changeWallBlock(b.wallID);
    m_actualBlock->changeWallBlockOrientation(wallRot, wallXFliped, wallYFliped);

    m_topBlocks->setBlock(b.topID);
    Rotation topRot(getRotation(b.topOrientation));
    if(topRot == Rotation::ROT_0)
        m_topRot0Widget->setChecked(true);
    else if(topRot == Rotation::ROT_90)
        m_topRot90Widget->setChecked(true);
    else if(topRot == ROT_180)
        m_topRot180Widget->setChecked(true);
    else m_topRot270Widget->setChecked(true);
    bool topXFliped(getXFlip(b.topOrientation));
    bool topYFliped(getYFlip(b.topOrientation));
    m_topXFlipedWidget->setChecked(topXFliped);
    m_topYFlipedWidget->setChecked(topYFliped);

    m_actualBlock->changeTopBlock(b.topID);
    m_actualBlock->changeTopBlockOrientation(topRot, topXFliped, topYFliped);
}

void BlockDock::onTileConfigChanged()
{
    m_groundBlocks->changeView(TILE_GROUND);
    m_wallBlocks->changeView(TILE_WALL);
    m_topBlocks->changeView(TILE_TOP);

    emit tileConfigChanged();
}
