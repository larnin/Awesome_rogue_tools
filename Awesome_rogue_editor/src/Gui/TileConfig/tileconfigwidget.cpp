#include "tileconfigwidget.h"
#include "Utilities/configs.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QFileDialog>
#include <QMessageBox>
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
    m_isTop = new QCheckBox("Bloc de plafond");
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
    frameLayout->addWidget(m_isTop);
    frameLayout->addSpacing(5);
    frameLayout->addWidget(m_isWall);
    QGroupBox* frame = new QGroupBox();
    frame->setLayout(frameLayout);
    frame->setFixedWidth(250);
    frame->setTitle("Configuration du bloc"); 

    m_normalDirWidget = new QLineEdit(QString::fromStdString(Configs::tiles.material.normal.getFilename()));
    m_searchNormalButton = new QPushButton("...");
    m_searchNormalButton->setFixedWidth(40);
    m_resetNormalButton = new QPushButton("Reset");
    m_ambiantWidget = new QDoubleSpinBox();
        m_ambiantWidget->setRange(0, 10);
        m_ambiantWidget->setDecimals(2);
        m_ambiantWidget->setSingleStep(0.1);
        m_ambiantWidget->setValue(Configs::tiles.material.ambiantCoeficient);
    m_diffuseWidget = new QDoubleSpinBox();
        m_diffuseWidget->setRange(0, 10);
        m_diffuseWidget->setDecimals(2);
        m_diffuseWidget->setSingleStep(0.1);
        m_diffuseWidget->setValue(Configs::tiles.material.diffuseCoefficient);
    m_specularWidget = new QDoubleSpinBox();
        m_specularWidget->setRange(0, 10);
        m_specularWidget->setDecimals(2);
        m_specularWidget->setSingleStep(0.1);
        m_specularWidget->setValue(Configs::tiles.material.specularCoefficient);
    m_multiplierWidget = new QDoubleSpinBox();
        m_multiplierWidget->setRange(0, 100);
        m_multiplierWidget->setDecimals(2);
        m_multiplierWidget->setSingleStep(0.1);
        m_multiplierWidget->setValue(Configs::tiles.material.specularMultiplier);
    QHBoxLayout* normalLayout = new QHBoxLayout();
    normalLayout->addWidget(new QLabel("Normal map :"));
    normalLayout->addStretch(1);
    normalLayout->addWidget(m_resetNormalButton);
    QHBoxLayout* normal2Layout = new QHBoxLayout();
    normal2Layout->addSpacing(10);
    normal2Layout->addWidget(m_normalDirWidget, 1);
    normal2Layout->addWidget(m_searchNormalButton);
    QFormLayout* materialFormLayout = new QFormLayout();
    materialFormLayout->addRow("Ambiant :", m_ambiantWidget);
    materialFormLayout->addRow("Diffuse :", m_diffuseWidget);
    materialFormLayout->addRow("Specular :", m_specularWidget);
    materialFormLayout->addRow("Spec mull :", m_multiplierWidget);
    QVBoxLayout* materialLayout = new QVBoxLayout();
    materialLayout->addLayout(normalLayout);
    materialLayout->addLayout(normal2Layout);
    materialLayout->addSpacing(5);
    materialLayout->addLayout(materialFormLayout);
    QGroupBox* materialBox = new QGroupBox("Material");
    materialBox->setLayout(materialLayout);

    QVBoxLayout* widgetsLayout = new QVBoxLayout();
    widgetsLayout->addLayout(buttonsLayout);
    widgetsLayout->addSpacing(10);
    widgetsLayout->addLayout(tileSizeLayout);
    widgetsLayout->addWidget(frame);
    widgetsLayout->addWidget(materialBox);
    widgetsLayout->addStretch(1);

    m_textureDirWidget = new QLineEdit(QString::fromStdString(Configs::tiles.texture.getFilename()));
    m_textureDirWidget->setReadOnly(true);
    m_searchTextureButton = new QPushButton("...");
    m_searchTextureButton->setFixedWidth(40);
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
    connect(m_isTop, SIGNAL(toggled(bool)), this, SLOT(onValueChanged()));
    connect(m_isWall, SIGNAL(toggled(bool)), this, SLOT(onValueChanged()));
    connect(m_wallBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(onValueChanged()));
    connect(m_rot0Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));
    connect(m_rot90Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));
    connect(m_rot180Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));
    connect(m_rot270Box, SIGNAL(clicked(bool)), this, SLOT(onValueChanged()));

    connect(m_ambiantWidget, SIGNAL(valueChanged(double)), this, SLOT(onMaterialValueChanged()));
    connect(m_diffuseWidget, SIGNAL(valueChanged(double)), this, SLOT(onMaterialValueChanged()));
    connect(m_specularWidget, SIGNAL(valueChanged(double)), this, SLOT(onMaterialValueChanged()));
    connect(m_multiplierWidget, SIGNAL(valueChanged(double)), this, SLOT(onMaterialValueChanged()));

    onTileSelected(0);
}

void TileConfigWidget::onTileSelected(unsigned int id)
{
    m_isGround->blockSignals(true);
    m_isWall->blockSignals(true);
    m_isTop->blockSignals(true);
    m_wallBoxType->blockSignals(true);
    m_rot0Box->blockSignals(true);
    m_rot90Box->blockSignals(true);
    m_rot180Box->blockSignals(true);
    m_rot270Box->blockSignals(true);

    m_currentTileID = id;

    m_tileLabel->setText("Tile ID : " + QString::number(m_currentTileID));

    auto itGround(std::find(Configs::tiles.groundIDs.begin(), Configs::tiles.groundIDs.end(), m_currentTileID));
    auto isTop(std::find(Configs::tiles.topIDs.begin(), Configs::tiles.topIDs.end(), m_currentTileID));
    auto itWall(std::find_if(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [id](const auto & v){return v.id == id;}));

    m_isGround->setChecked(itGround != Configs::tiles.groundIDs.end());
    m_isTop->setChecked(isTop != Configs::tiles.topIDs.end());
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
    m_isTop->blockSignals(false);
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

    if(m_isTop->isChecked())
    {
        if(std::find(Configs::tiles.topIDs.begin(), Configs::tiles.topIDs.end(), m_currentTileID) == Configs::tiles.topIDs.end())
            Configs::tiles.topIDs.push_back(m_currentTileID);
    }
    else
    {
        auto it(std::find(Configs::tiles.topIDs.begin(), Configs::tiles.topIDs.end(), m_currentTileID));
        if(it != Configs::tiles.topIDs.end())
        {
            std::swap(*it, Configs::tiles.topIDs.back());
            Configs::tiles.topIDs.pop_back();
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

    std::sort(Configs::tiles.groundIDs.begin(), Configs::tiles.groundIDs.end());
    std::sort(Configs::tiles.topIDs.begin(), Configs::tiles.topIDs.end());
    std::sort(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [](const auto & a, const auto & b){return a.id < b.id;});

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

void TileConfigWidget::onSearchNormal()
{
    QString fileName(QFileDialog::getOpenFileName(this, "Selectionner une normal map ..."));
    if(fileName.isEmpty())
        return;

    Texture t(fileName.toStdString());
    if(!t.isValid())
    {
        QMessageBox::information(this, "Normal map non chargée", "La normal map indiquée n'a pas pu etre chargée !");
        return;
    }

    Configs::tiles.material.normal = t;
    m_normalDirWidget->setText(fileName);

    emit tileConfigChanged();
}

void TileConfigWidget::onResetNormal()
{
    Configs::tiles.material.normal = defaultNormalMap();
    m_normalDirWidget->setText("");

    emit tileConfigChanged();
}

void TileConfigWidget::onMaterialValueChanged()
{
    Configs::tiles.material.ambiantCoeficient = m_ambiantWidget->value();
    Configs::tiles.material.diffuseCoefficient = m_diffuseWidget->value();
    Configs::tiles.material.specularCoefficient = m_specularWidget->value();
    Configs::tiles.material.specularMultiplier = m_multiplierWidget->value();

    emit tileConfigChanged();
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
