#include "tileconfigwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QFileDialog>
#include <QMessageBox>
#include "Utilities/configs.h"
#include <algorithm>

TileConfigWidget::TileConfigWidget(QWidget * parent)
    : QWidget(parent)
    , m_currentTileID(0)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("Configurer les blocks");

    resize(800, 500);

    m_importButton = new QPushButton("Importer");
    m_exportButton = new QPushButton("Exporter");
    m_closeButton = new QPushButton("Fermer");
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_importButton);
    buttonsLayout->addWidget(m_exportButton);
    buttonsLayout->addWidget(m_closeButton);

    m_tileSizeBox = new QSpinBox();
    m_tileSizeBox->setRange(8, 256);
    m_tileSizeBox->setValue(Configs::tiles.tileSize);
    m_tileSizeBox->setFixedWidth(100);
    QHBoxLayout* tileSizeLayout = new QHBoxLayout();
    tileSizeLayout->addWidget(new QLabel("Taille de bloc :"));
    tileSizeLayout->addWidget(m_tileSizeBox, 1, Qt::AlignLeft);

    m_tileLabel = new QLabel("Tile ID : 0");
    auto f(m_tileLabel->font());
    f.setPixelSize(16);
    m_tileLabel->setFont(f);

    m_isGround = new QCheckBox("Bloc de sol");
    m_isWall = new QGroupBox("Bloc de mur");
    m_isWall->setCheckable(true);
    m_isWall->setChecked(false);

    m_wallBoxType = new QComboBox();
    m_wallBoxType->addItem("Vide");
    m_wallBoxType->addItem("Plein");
    m_wallBoxType->addItem("Triangle");
    m_wallBoxType->addItem("Demi");
    m_wallBoxType->addItem("Quart");
    m_wallBoxType->addItem("3 quart");
    m_wallBoxType->addItem("Demi centré");
    m_wallBoxType->addItem("Croix");
    m_wallBoxType->addItem("Forme T");
    m_wallBoxType->addItem("Coin centré");

    m_rot0Box = new QRadioButton("0°");
    m_rot90Box = new QRadioButton("90°");
    m_rot180Box = new QRadioButton("180°");
    m_rot270Box = new QRadioButton("270°");
    QHBoxLayout *rotLayout = new QHBoxLayout();
    rotLayout->addWidget(m_rot0Box);
    rotLayout->addWidget(m_rot90Box);
    rotLayout->addWidget(m_rot180Box);
    rotLayout->addWidget(m_rot270Box);
    QGroupBox* frameRot = new QGroupBox("Rotation de la hitbox");
    frameRot->setLayout(rotLayout);

    m_wallViewer = new TileViewer();
    m_wallViewer->setFixedSize(70, 70);
    m_wallViewer->setTexture(Configs::tiles.texture);
    m_wallViewer->setTileSize(Configs::tiles.tileSize);
    QVBoxLayout* frameWallViewerLayout = new QVBoxLayout();
    frameWallViewerLayout->addWidget(m_wallViewer);
    QFrame* frameWallViewer = new QFrame();
    frameWallViewer->setLayout(frameWallViewerLayout);
    frameWallViewer->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout* wallLayout = new QVBoxLayout();
    wallLayout->addWidget(m_wallBoxType);
    wallLayout->addSpacing(5);
    wallLayout->addWidget(frameRot);
    wallLayout->addSpacing(5);
    wallLayout->addWidget(frameWallViewer, 0, Qt::AlignHCenter);
    m_isWall->setLayout(wallLayout);

    QVBoxLayout* frameLayout = new QVBoxLayout();
    frameLayout->addWidget(m_tileLabel);
    frameLayout->addSpacing(10);
    frameLayout->addWidget(m_isGround);
    frameLayout->addSpacing(5);
    frameLayout->addWidget(m_isWall);
    QGroupBox* frame = new QGroupBox();
    frame->setLayout(frameLayout);
    frame->setFixedWidth(250);
    frame->setTitle("Configuration du bloc");
    QVBoxLayout* widgetsLayout = new QVBoxLayout();
    widgetsLayout->addLayout(buttonsLayout);
    widgetsLayout->addSpacing(10);
    widgetsLayout->addLayout(tileSizeLayout);
    widgetsLayout->addWidget(frame);
    widgetsLayout->addStretch(1);

    m_textureDirWidget = new QLineEdit(QString::fromStdString(Configs::tiles.texture.getFilename()));
    m_textureDirWidget->setReadOnly(true);
    m_searchTextureButton = new QPushButton("...");
    QHBoxLayout* textureStrLayout = new QHBoxLayout();
    textureStrLayout->addWidget(m_textureDirWidget, 1);
    textureStrLayout->addWidget(m_searchTextureButton);

    m_textureViewerWidget = new TextureViewer();
    m_textureViewerWidget->setTexture(Configs::tiles.texture);
    m_textureViewerWidget->setTileSize(Configs::tiles.tileSize);
    QVBoxLayout* textureLayout = new QVBoxLayout();
    textureLayout->addLayout(textureStrLayout);
    textureLayout->addSpacing(5);
    textureLayout->addWidget(m_textureViewerWidget, 1);

    QWidget* spacer = new QWidget();
    spacer->setFixedWidth(2);
    spacer->setStyleSheet(QString("background-color: #b0b0b0;"));
    QHBoxLayout* centralLayout = new QHBoxLayout();
    centralLayout->addLayout(textureLayout, 1);
    centralLayout->addWidget(spacer);
    centralLayout->addLayout(widgetsLayout);

    setLayout(centralLayout);

    connect(m_textureViewerWidget, SIGNAL(tileSelected(uint)), this, SLOT(onTileSelected(uint)));
    connect(m_searchTextureButton, SIGNAL(clicked()), this, SLOT(onSearchTexture()));
    connect(m_importButton, SIGNAL(clicked()), this, SLOT(onImportClicked()));
    connect(m_exportButton, SIGNAL(clicked()), this, SLOT(onExportClicked()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_tileSizeBox, SIGNAL(valueChanged(int)), this, SLOT(onTileSizeChanged(int)));

    connect(m_isGround, SIGNAL(toggled(bool)), this, SLOT(onValueChanged()));
    connect(m_isWall, SIGNAL(toggled(bool)), this, SLOT(onValueChanged()));
    connect(m_wallBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(onValueChanged()));
    connect(m_rot0Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));
    connect(m_rot90Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));
    connect(m_rot180Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));
    connect(m_rot270Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));

    onTileSelected(0);
}

void TileConfigWidget::onTileSelected(unsigned int id)
{
    m_isGround->blockSignals(true);
    m_isWall->blockSignals(true);
    m_wallBoxType->blockSignals(true);
    m_rot0Box->blockSignals(true);
    m_rot90Box->blockSignals(true);
    m_rot180Box->blockSignals(true);
    m_rot270Box->blockSignals(true);

    m_currentTileID = id;

    m_tileLabel->setText("Tile ID : " + QString::number(m_currentTileID));

    auto itGround(std::find(Configs::tiles.groundIDs.begin(), Configs::tiles.groundIDs.end(), m_currentTileID));
    auto itWall(std::find_if(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [id](const auto & v){return v.id == id;}));

    m_isGround->setChecked(itGround != Configs::tiles.groundIDs.end());
    if(itWall!= Configs::tiles.walls.end())
    {
        const TileWallInfo & tile(*itWall);
        m_isWall->setChecked(true);
        if(tile.hitboxRot == ROT_0)
            m_rot0Box->setChecked(true);
        else if(tile.hitboxRot == ROT_90)
            m_rot90Box->setChecked(true);
        else if(tile.hitboxRot == ROT_180)
            m_rot180Box->setChecked(true);
        else m_rot270Box->setChecked(true);

        if(tile.hitbox == EMPTY)
            m_wallBoxType->setCurrentIndex(0);
        else if(tile.hitbox == FULL)
            m_wallBoxType->setCurrentIndex(1);
        else if(tile.hitbox == TRIANGLE)
            m_wallBoxType->setCurrentIndex(2);
        else if(tile.hitbox == HALF)
            m_wallBoxType->setCurrentIndex(3);
        else if(tile.hitbox == QUARTER)
            m_wallBoxType->setCurrentIndex(4);
        else if(tile.hitbox == THREE_QUARTER)
            m_wallBoxType->setCurrentIndex(5);
        else if(tile.hitbox == HALF_CENTRED)
            m_wallBoxType->setCurrentIndex(6);
        else if(tile.hitbox == CROSS)
            m_wallBoxType->setCurrentIndex(7);
        else if(tile.hitbox == T_SHARP)
            m_wallBoxType->setCurrentIndex(8);
        else  //CORNER_CENTRED
            m_wallBoxType->setCurrentIndex(9);

        m_wallViewer->setBlockInfo(m_currentTileID, tile.hitbox, tile.hitboxRot);
    }
    else
    {
        m_isWall->setChecked(false);
        m_rot0Box->setChecked(true);
        m_wallBoxType->setCurrentIndex(0);
        m_wallViewer->setBlockInfo(m_currentTileID, EMPTY, ROT_0);
    }

    m_isGround->blockSignals(false);
    m_isWall->blockSignals(false);
    m_wallBoxType->blockSignals(false);
    m_rot0Box->blockSignals(false);
    m_rot90Box->blockSignals(false);
    m_rot180Box->blockSignals(false);
    m_rot270Box->blockSignals(false);

}

void TileConfigWidget::onValueChanged()
{
    if(m_isGround->isChecked())
    {
        if(std::find(Configs::tiles.groundIDs.begin(), Configs::tiles.groundIDs.end(), m_currentTileID) == Configs::tiles.groundIDs.end())
            Configs::tiles.groundIDs.push_back(m_currentTileID);
    }
    else
    {
        auto it(std::find(Configs::tiles.groundIDs.begin(), Configs::tiles.groundIDs.end(), m_currentTileID));
        if(it != Configs::tiles.groundIDs.end())
        {
            std::swap(*it, Configs::tiles.groundIDs.back());
            Configs::tiles.groundIDs.pop_back();
        }
    }

    unsigned int id(m_currentTileID);
    if(m_isWall->isChecked())
    {
        TileWallInfo tile(createTile());
        auto it(std::find_if(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [id](const auto & v){return v.id == id;}));
        if(it != Configs::tiles.walls.end())
            *it = tile;
        else Configs::tiles.walls.push_back(tile);
        m_wallViewer->setBlockInfo(tile.id, tile.hitbox, tile.hitboxRot);
    }
    else
    {
        auto it(std::find_if(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [id](const auto & v){return v.id == id;}));
        if(it != Configs::tiles.walls.end())
        {
            std::swap(*it, Configs::tiles.walls.back());
            Configs::tiles.walls.pop_back();
        }
        m_wallViewer->setBlockInfo(m_currentTileID, EMPTY, ROT_0);
    }

    emit tileConfigChanged();
}

void TileConfigWidget::onTileSizeChanged(int value)
{
    m_wallViewer->setTileSize(value);
    m_textureViewerWidget->setTileSize(value);
    Configs::tiles.tileSize = value;
    emit tileConfigChanged();
}

void TileConfigWidget::onSearchTexture()
{
    QString fileName(QFileDialog::getOpenFileName(this, "Selectionner une texture ..."));
    if(fileName.isEmpty())
        return;

    Texture t(fileName.toStdString());
    if(!t.isValid())
    {
        QMessageBox::information(this, "Texture non chargée", "La texture indiquée n'a pas pu etre chargée !");
        return;
    }

    Configs::tiles.texture = t;
    m_textureDirWidget->setText(fileName);
    m_textureViewerWidget->setTexture(t);
    m_wallViewer->setTexture(t);

    emit tileConfigChanged();
}

void TileConfigWidget::onImportClicked()
{
    QString fileName(QFileDialog::getOpenFileName(this, "Importer ...", QString(), "*.json"));
    if(fileName.isEmpty())
        return;
    Configs::tiles = TileConfig(fileName);
    onTileSelected(m_currentTileID);
    emit tileConfigChanged();
}

void TileConfigWidget::onExportClicked()
{
    QString fileName(QFileDialog::getSaveFileName(this, "Exporter ...", QString(), "*json"));
    if(fileName.isEmpty())
        return;
    if(!fileName.endsWith(".json"))
        fileName += ".json";
    Configs::tiles.save(fileName);
}

TileWallInfo TileConfigWidget::createTile() const
{
    Rotation rot;
    if(m_rot0Box->isChecked())
        rot = ROT_0;
    else if(m_rot90Box->isChecked())
        rot = ROT_90;
    else if(m_rot180Box->isChecked())
        rot = ROT_180;
    else rot = ROT_270;

    BoxType type;
    if(m_wallBoxType->currentIndex() == 0) //"Vide"
        type = EMPTY;
    else if(m_wallBoxType->currentIndex() == 1) //"Plein"
        type = FULL;
    else if(m_wallBoxType->currentIndex() == 2) //"Triangle"
        type = TRIANGLE;
    else if(m_wallBoxType->currentIndex() == 3) //"Demi"
        type = HALF;
    else if(m_wallBoxType->currentIndex() == 4) //"Quart"
        type = QUARTER;
    else if(m_wallBoxType->currentIndex() == 5) //"3 quart"
        type = THREE_QUARTER;
    else if(m_wallBoxType->currentIndex() == 6) //"Demi centré"
        type = HALF_CENTRED;
    else if(m_wallBoxType->currentIndex() == 7) //"Croix"
        type = CROSS;
    else if(m_wallBoxType->currentIndex() == 8) //"Forme T"
        type = T_SHARP;
    else //"Coin centré"
        type = CORNER_CENTRED;

    return TileWallInfo(m_currentTileID, rot, type);
}
