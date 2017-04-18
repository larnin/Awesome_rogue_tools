#include "animatorconfig.h"
#include "tilesetdata.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QLabel>
#include <QGroupBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>

AnimatorConfig::AnimatorConfig(QWidget * parent)
    : QWidget(parent)
{
    setWindowTitle("Animation creator");

    m_tileSizeWidget = new QSpinBox();
    m_tileSizeWidget->setRange(8, 512);
    m_tileSizeWidget->setValue(TilesetData::instance.tileSize);
    QHBoxLayout *tileSizeLayout = new QHBoxLayout();
    tileSizeLayout->addWidget(new QLabel("Taille de bloc :"));
    tileSizeLayout->addWidget(m_tileSizeWidget, 1);

    m_loadButton = new QPushButton("Charger");
    m_saveButton = new QPushButton("Sauvegarder");
    m_clearButton = new QPushButton("Effacer");
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_loadButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_clearButton);

    m_searchTexture = new QPushButton("...");
    m_searchTexture->setFixedWidth(40);
    m_textureNameWidget = new QLineEdit();
    m_textureNameWidget->setReadOnly(true);
    QHBoxLayout * texLayout = new QHBoxLayout();
    texLayout->addWidget(m_textureNameWidget);
    texLayout->addWidget(m_searchTexture);

    m_blocksWidget = new QListWidget();
    m_animsWidget = new QListWidget();
    m_framesWidget = new QListWidget();

    m_rot0 = new QRadioButton("0°");
    m_rot90 = new QRadioButton("90°");
    m_rot180 = new QRadioButton("180°");
    m_rot270 = new QRadioButton("270°");

    QHBoxLayout* rotLayout = new QHBoxLayout();
    rotLayout->addWidget(m_rot0);
    rotLayout->addWidget(m_rot90);
    rotLayout->addWidget(m_rot180);
    rotLayout->addWidget(m_rot270);
    QGroupBox* rotBox = new QGroupBox("Rotation");
    rotBox->setLayout(rotLayout);

    m_idFrame = new QSpinBox();
    m_idFrame->setRange(0, 1024);
    m_flipXFrame = new QCheckBox("Flip X");
    m_flipYFrame = new QCheckBox("Flip Y");
    QHBoxLayout *flipLayout = new QHBoxLayout();
    flipLayout->addWidget(m_flipXFrame);
    flipLayout->addWidget(m_flipYFrame);

    m_minTimeFrame = new QDoubleSpinBox();
    m_minTimeFrame->setDecimals(3);
    m_minTimeFrame->setRange(0.001, 60);
    m_maxTimeFrame = new QDoubleSpinBox();
    m_maxTimeFrame->setDecimals(3);
    m_maxTimeFrame->setRange(0.001, 60);

    QVBoxLayout *frameLayout = new QVBoxLayout();
    frameLayout->addWidget(m_idFrame);
    frameLayout->addWidget(rotBox);
    frameLayout->addLayout(flipLayout);
    frameLayout->addSpacing(5);
    frameLayout->addWidget(m_minTimeFrame);
    frameLayout->addWidget(m_maxTimeFrame);
    m_frameBox = new QGroupBox("Frame :");
    m_frameBox->setDisabled(true);
    m_frameBox->setLayout(frameLayout);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addLayout(tileSizeLayout);
    layout->addSpacing(5);
    layout->addLayout(buttonLayout);
    layout->addLayout(texLayout);
    layout->addSpacing(10);
    layout->addWidget(new QLabel("Blocs :"));
    layout->addWidget(m_blocksWidget);
    layout->addSpacing(5);
    layout->addWidget(new QLabel("Animations :"));
    layout->addWidget(m_animsWidget);
    layout->addSpacing(5);
    layout->addWidget(new QLabel("Frames :"));
    layout->addWidget(m_framesWidget);
    layout->addWidget(m_frameBox);
    setLayout(layout);

    m_blocksWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_blocksWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClickBlocks(QPoint)));
    m_animsWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_animsWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClickAnims(QPoint)));
    m_framesWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_framesWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClickFrames(QPoint)));

    connect(m_idFrame, SIGNAL(valueChanged(int)), this, SLOT(frameValueChanged()));
    connect(m_flipXFrame, SIGNAL(clicked(bool)), this, SLOT(frameValueChanged()));
    connect(m_flipYFrame, SIGNAL(clicked(bool)), this, SLOT(frameValueChanged()));
    connect(m_rot0, SIGNAL(clicked(bool)), this, SLOT(frameValueChanged()));
    connect(m_rot90, SIGNAL(clicked(bool)), this, SLOT(frameValueChanged()));
    connect(m_rot180, SIGNAL(clicked(bool)), this, SLOT(frameValueChanged()));
    connect(m_rot270, SIGNAL(clicked(bool)), this, SLOT(frameValueChanged()));
    connect(m_minTimeFrame, SIGNAL(valueChanged(double)), this, SLOT(frameValueChanged()));
    connect(m_maxTimeFrame, SIGNAL(valueChanged(double)), this, SLOT(frameValueChanged()));

    connect(m_blocksWidget, SIGNAL(currentRowChanged(int)), this, SLOT(reloadFromAnims()));
    connect(m_animsWidget, SIGNAL(currentRowChanged(int)), this, SLOT(reloadFromFrames()));
    connect(m_framesWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateFrameData()));

    connect(m_loadButton, SIGNAL(clicked(bool)), this, SLOT(loadClicked()));
    connect(m_saveButton, SIGNAL(clicked(bool)), this, SLOT(saveClicked()));
    connect(m_clearButton, SIGNAL(clicked(bool)), this, SLOT(clearClicked()));
    connect(m_searchTexture, SIGNAL(clicked(bool)), this, SLOT(searchClicked()));

    connect(this, SIGNAL(listChanged()), &m_viewer, SIGNAL(onConfigChanged()));
    connect(this, SIGNAL(listChanged()), &m_animator, SLOT(onConfigChanged()));
}

void AnimatorConfig::onRightClickBlocks(const QPoint& point)
{
    QPoint globalPos(m_blocksWidget->viewport()->mapToGlobal(point));

    QAction *aAdd(nullptr), *aDel(nullptr), *aChange(nullptr);
    QMenu menu;
    aAdd = menu.addAction("Ajouter un bloc");
    if(m_blocksWidget->currentRow() >= 0 && m_blocksWidget->currentRow() < m_blocksWidget->count())
    {
        aDel = menu.addAction("Supprimer le bloc");
        aChange = menu.addAction("Changer l'id");
    }
    QAction* action = menu.exec(globalPos);
    if(action == nullptr)
        return;

    auto & list(TilesetData::instance.animations);
    if(action == aAdd)
    {
        bool ok(false);
        int id(QInputDialog::getInt(this, "Ajouter un bloc", "Id du bloc :", 0, 0, 1024, 1, &ok));
        if(!ok)
            return;
        if(std::find_if(list.begin(), list.end(), [id](const auto & a){return int(a.id) == id;}) != list.end())
        {
            QMessageBox::information(this, "ID existant", "L'id " + QString::number(id) + " est déja animé !");
            return;
        }
        list.push_back(Animation(id));
        reloadFromBlocks();
        emit listChanged();
    }
    else if(action == aDel)
    {
        int blockIndex(m_blocksWidget->currentRow());
        if(blockIndex < 0 || blockIndex >= int(list.size()))
            return;
        int result(QMessageBox::question(this, "Supprimer", "Etes vous sur de vouloir supprimer l'animation du bloc " + QString::number(list[blockIndex].id) + " ?", QMessageBox::Yes, QMessageBox::No));
        if(result != QMessageBox::Yes)
            return;
        list.erase(list.begin()+blockIndex);
        reloadFromBlocks();
        emit listChanged();
    }
    else if(action == aChange)
    {
        int blockIndex(m_blocksWidget->currentRow());
        if(blockIndex < 0 || blockIndex >= int(list.size()))
            return;
        bool ok(false);
        int id(QInputDialog::getInt(this, "Changer l'id du bloc " + QString::number(list[blockIndex].id), "Nouvel id : ", list[blockIndex].id, 0, 1024, 1, &ok));
        if(!ok)
            return;

        if(std::find_if(list.begin(), list.end(), [id](const auto & a){return int(a.id) == id;}) != list.end())
        {
            QMessageBox::information(this, "ID existant", "L'id " + QString::number(id) + " est déja animé !");
            return;
        }

        list[blockIndex].id = id;
        reloadFromBlocks();
        emit listChanged();
    }
}

void AnimatorConfig::onRightClickAnims(const QPoint& point)
{
    auto & list(TilesetData::instance.animations);
    int blockIndex(m_blocksWidget->currentRow());
    if(blockIndex < 0 || blockIndex >= int(list.size()))
        return;

    QPoint globalPos(m_animsWidget->viewport()->mapToGlobal(point));

    QAction *aAdd(nullptr), *aDel(nullptr), *aChange(nullptr);
    QMenu menu;
    aAdd = menu.addAction("Ajouter une animation");
    if(m_animsWidget->currentRow() >= 0 && m_animsWidget->currentRow() < m_animsWidget->count())
    {
        aDel = menu.addAction("Supprimer l'animation");
        aChange = menu.addAction("Changer le poid");
    }
    QAction* action = menu.exec(globalPos);
    if(action == nullptr)
        return;

    auto & list2(list[blockIndex].anims);
    if(action == aAdd)
    {
        bool ok(false);
        float weight(QInputDialog::getDouble(this, "Ajouter une animation", "Poid de l'animation :", 1, 1, 100, 1, &ok));
        if(!ok)
            return;
        list2.push_back(SingleAnimation(weight));
        reloadFromAnims();
        emit listChanged();
    }
    else if(action == aDel)
    {
        int animIndex(m_animsWidget->currentRow());
        if(animIndex < 0 || animIndex >= int(list2.size()))
            return;
        int result(QMessageBox::question(this, "Supprimer", "Etes vous sur de vouloir supprimer cette animation ?", QMessageBox::Yes, QMessageBox::No));
        if(result != QMessageBox::Yes)
            return;
        list2.erase(list2.begin()+animIndex);
        reloadFromAnims();
        emit listChanged();
    }
    else if(action == aChange)
    {
        int animIndex(m_animsWidget->currentRow());
        if(animIndex < 0 || animIndex >= int(list2.size()))
            return;
        bool ok(false);
        double weight(QInputDialog::getDouble(this, "Changer le poid de l'animation" , "Nouveau poid : ", list2[animIndex].weight, 1, 100, 1, &ok));
        if(!ok)
            return;

        list2[animIndex].weight = weight;
        reloadFromAnims();
        emit listChanged();
    }
}

void AnimatorConfig::onRightClickFrames(const QPoint& point)
{
    auto & list(TilesetData::instance.animations);
    int blockIndex(m_blocksWidget->currentRow());
    if(blockIndex < 0 || blockIndex >= int(list.size()))
        return;
    auto & list2(list[blockIndex].anims);
    int animIndex(m_animsWidget->currentRow());
    if(animIndex < 0 || animIndex >= int(list2.size()))
        return;

    QPoint globalPos(m_framesWidget->viewport()->mapToGlobal(point));

    QAction *aAdd(nullptr), *aDel(nullptr), *aUp(nullptr), *aDown(nullptr), *aDuplique(nullptr);
    QMenu menu;
    aAdd = menu.addAction("Ajouter une frame");
    if(m_framesWidget->currentRow() >= 0 && m_framesWidget->currentRow() < m_framesWidget->count())
    {
        aDel = menu.addAction("Suprimer la frame");
        aDuplique = menu.addAction("Dupliquer la frame");
        if(m_framesWidget->currentRow() >= 1)
            aUp = menu.addAction("Monter la frame");
        if(m_framesWidget->currentRow() < m_framesWidget->count()-1)
            aDown = menu.addAction("Descendre la frame");

    }
    QAction* action = menu.exec(globalPos);
    if(action == nullptr)
        return;

    auto & list3(list2[animIndex].frames);
    if(action == aAdd)
    {
        list3.push_back(Frame());
        reloadFromFrames();
        emit listChanged();
    }
    else if(action == aDel)
    {
        int frameIndex(m_framesWidget->currentRow());
        if(frameIndex < 0 || frameIndex >= int(list3.size()))
            return;

        list3.erase(list3.begin()+frameIndex);
        reloadFromFrames();
        emit listChanged();
    }
    else if(action == aUp)
    {
        int frameIndex(m_framesWidget->currentRow());
        if(frameIndex < 1 || frameIndex >= int(list3.size()))
            return;
        std::swap(list3[frameIndex-1], list3[frameIndex]);
        reloadFromFrames();
        updateFrameData();
        emit listChanged();
    }
    else if(action == aDown)
    {
        int frameIndex(m_framesWidget->currentRow());
        if(frameIndex < 0 || frameIndex >= int(list3.size()-1))
            return;
        std::swap(list3[frameIndex], list3[frameIndex+1]);
        reloadFromFrames();
        updateFrameData();
        emit listChanged();
    }
    else if(action == aDuplique)
    {
        int frameIndex(m_framesWidget->currentRow());
        if(frameIndex < 0 || frameIndex >= int(list3.size()))
            return;
        list3.insert(list3.begin()+frameIndex, list3[frameIndex]);
        reloadFromFrames();
        emit listChanged();
    }
}

void AnimatorConfig::reloadFromBlocks()
{
    int blockIndex(m_blocksWidget->currentRow());
    blockSignalsFromBlocks(true);

    m_blocksWidget->clear();
    for(const Animation & a : TilesetData::instance.animations)
        m_blocksWidget->addItem("Bloc " + QString::number(a.id) + " (" + QString::number(a.anims.size()) + " animations)");

    blockSignalsFromBlocks(false);
    m_blocksWidget->setCurrentRow(blockIndex);

    reloadFromAnims();
}

void AnimatorConfig::reloadFromAnims()
{
    int blockIndex(m_blocksWidget->currentRow());
    int animIndex(m_animsWidget->currentRow());
    blockSignalsFromAnims(true);

    m_animsWidget->clear();
    const auto & list(TilesetData::instance.animations);
    unsigned int i(1);
    if(blockIndex >= 0 && blockIndex < int(list.size()))
    {
        for(const SingleAnimation & a : list[blockIndex].anims)
            m_animsWidget->addItem("Animation " + QString::number(i++) + " - Poid " + QString::number(a.weight) + " (" + QString::number(a.frames.size()) + " frames)");
    }

    blockSignalsFromAnims(false);
    m_animsWidget->setCurrentRow(animIndex);

    reloadFromFrames();
}

void AnimatorConfig::reloadFromFrames()
{
    int blockIndex(m_blocksWidget->currentRow());
    int animIndex(m_animsWidget->currentRow());
    int frameIndex(m_framesWidget->currentRow());
    blockSignalsFromFrames(true);

    m_frameBox->setDisabled(true);

    m_framesWidget->clear();
    const auto & list(TilesetData::instance.animations);
    unsigned int i(1);
    if(blockIndex >= 0 && blockIndex < int(list.size()))
    {
        const auto & list2(list[blockIndex].anims);
        if(animIndex >= 0 && animIndex < int(list2.size()))
        {
            for(const Frame & f : list2[animIndex].frames)
            {
                if(int(i)-1 == frameIndex)
                    m_frameBox->setEnabled(true);

                QString timestr(std::abs(f.minTime-f.maxTime) < 0.0015f
                                ? "(temps " + QString::number(f.minTime) + ")"
                                : "(temps " + QString::number(f.minTime) + "/" + QString::number(f.maxTime) + ")");
                m_framesWidget->addItem("Frame " + QString::number(i++) + " - ID " + QString::number(f.id) + " " + timestr);
            }
        }
    }

    blockSignalsFromFrames(false);
    m_framesWidget->setCurrentRow(frameIndex);
}

void AnimatorConfig::updateFrameData()
{
    int blockIndex(m_blocksWidget->currentRow());
    int animIndex(m_animsWidget->currentRow());
    int frameIndex(m_framesWidget->currentRow());
    blockSignalsFromFrame(true);

    m_frameBox->setDisabled(true);
    const auto & list(TilesetData::instance.animations);
    if(blockIndex >= 0 && blockIndex < int(list.size()))
    {
        const auto & list2(list[blockIndex].anims);
        if(animIndex >= 0 && animIndex < int(list2.size()))
        {
            const auto & list3(list2[animIndex].frames);
            if(frameIndex >= 0 && frameIndex < int(list3.size()))
            {
                const Frame & f(list3[frameIndex]);

                m_frameBox->setEnabled(true);
                m_idFrame->setValue(f.id);
                m_flipXFrame->setChecked(f.flipX);
                m_flipYFrame->setChecked(f.flipY);
                m_rot0->setChecked(f.rot == ROT_0);
                m_rot90->setChecked(f.rot == ROT_90);
                m_rot180->setChecked(f.rot == ROT_180);
                m_rot270->setChecked(f.rot == ROT_270);
                m_minTimeFrame->setValue(f.minTime);
                m_maxTimeFrame->setValue(f.maxTime);
            }
        }
    }

    blockSignalsFromFrame(false);
    m_framesWidget->setCurrentRow(frameIndex);
}

void AnimatorConfig::blockSignalsFromBlocks(bool value)
{
    m_blocksWidget->blockSignals(value);
    blockSignalsFromAnims(value);
}

void AnimatorConfig::blockSignalsFromAnims(bool value)
{
    m_animsWidget->blockSignals(value);
    blockSignalsFromFrames(value);
}

void AnimatorConfig::blockSignalsFromFrames(bool value)
{
    m_framesWidget->blockSignals(value);
    blockSignalsFromFrame(value);
}

void AnimatorConfig::blockSignalsFromFrame(bool value)
{
    m_idFrame->blockSignals(value);
    m_flipXFrame->blockSignals(value);
    m_flipYFrame->blockSignals(value);
    m_rot0->blockSignals(value);
    m_rot90->blockSignals(value);
    m_rot180->blockSignals(value);
    m_rot270->blockSignals(value);
    m_minTimeFrame->blockSignals(value);
    m_maxTimeFrame->blockSignals(value);
}

void AnimatorConfig::frameValueChanged()
{
    Frame f(m_minTimeFrame->value(), m_maxTimeFrame->value(), m_idFrame->value()
            , m_rot0->isChecked() ? ROT_0 : m_rot90->isChecked() ? ROT_90 : m_rot180->isChecked() ? ROT_180 : ROT_270
            , m_flipXFrame->isChecked(), m_flipYFrame->isChecked());

    int blockIndex(m_blocksWidget->currentRow());
    int animIndex(m_animsWidget->currentRow());
    int frameIndex(m_framesWidget->currentRow());
    auto & list(TilesetData::instance.animations);
    if(blockIndex >= 0 && blockIndex < int(list.size()))
    {
        auto & list2(list[blockIndex].anims);
        if(animIndex >= 0 && animIndex < int(list2.size()))
        {
            auto & list3(list2[animIndex].frames);
            if(frameIndex >= 0 && frameIndex < int(list3.size()))
                list3[frameIndex] = f;
        }
    }

    reloadFromFrames();
    emit listChanged();
}

void AnimatorConfig::loadClicked()
{
    QString fileName(QFileDialog::getOpenFileName(this, "Ouvrir ...", QString(), "*.json"));
    if(fileName.isEmpty())
        return;

    TilesetData::instance.load(fileName);
    emit listChanged();

    reloadFromBlocks();
    reloadFromAnims();
    reloadFromFrames();
    m_textureNameWidget->setText(QString::fromStdString(TilesetData::instance.texture.getFilename()));
}

void AnimatorConfig::saveClicked()
{
    QString file(QFileDialog::getSaveFileName(this, "Sauvegarder ...", QString(), "*.json"));
       if(file.isEmpty())
           return;
    if(!file.endsWith(".json"))
        file+=".json";

    TilesetData::instance.save(file);
}

void AnimatorConfig::clearClicked()
{
    int result(QMessageBox::question(this, "Tout effacer", "Etes vous sur de vouloir tout supprimer ?", QMessageBox::Yes, QMessageBox::No));
    if(result != QMessageBox::Yes)
        return;

    TilesetData::instance.texture = Texture();
    TilesetData::instance.animations.clear();

    reloadFromBlocks();
    reloadFromAnims();
    reloadFromFrames();

    m_textureNameWidget->setText("");
}

void AnimatorConfig::searchClicked()
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

    TilesetData::instance.texture = t;
    m_textureNameWidget->setText(fileName);
    emit listChanged();
}

void AnimatorConfig::tileSizeChanged()
{
    TilesetData::instance.tileSize = m_tileSizeWidget->value();
    emit listChanged();
}

void AnimatorConfig::showEvent(QShowEvent *e)
{
    QSize s(QApplication::desktop()->screen(QApplication::desktop()->screenNumber(this))->size());
    QPoint p((QPoint(s.width(), s.height())-QPoint(size().width(), size().height()))/2);
    move(p);
    m_viewer.show();
    m_viewer.move(pos()+QPoint(size().width()+20, 0));
    m_animator.show();
    m_animator.move(pos()-QPoint(m_animator.size().width()+20, 0));
    QWidget::showEvent(e);
}

void AnimatorConfig::closeEvent(QCloseEvent *e)
{
    m_viewer.close();
    m_animator.close();
    QWidget::closeEvent(e);
}
