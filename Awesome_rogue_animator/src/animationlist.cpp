#include "animationlist.h"
#include "utilities.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>

AnimationList::AnimationList(QWidget *parent)
    : QWidget(parent)
{
    m_chooseTextureButton = new QPushButton("...");
    m_chooseTextureButton->setFixedWidth(30);
    m_textureText = new QLineEdit();
    m_textureText->setReadOnly(true);
    QHBoxLayout * textureLayout = new QHBoxLayout();
    textureLayout->addWidget(new QLabel("Texture :"));
    textureLayout->addSpacing(5);
    textureLayout->addWidget(m_textureText, 1);
    textureLayout->addSpacing(5);
    textureLayout->addWidget(m_chooseTextureButton);

    m_animationList = new QListWidget();
    m_frameList = new QListWidget();
    m_single = new QCheckBox(" Single shoot");

    m_rectLeft = new QDoubleSpinBox();
    m_rectLeft->setDecimals(0);
    m_rectLeft->setRange(0, 10000);
    m_rectTop = new QDoubleSpinBox();
    m_rectTop->setDecimals(0);
    m_rectTop->setRange(0, 10000);
    QHBoxLayout * rect1 = new QHBoxLayout();
    rect1->addWidget(new QLabel("X: "));
    rect1->addWidget(m_rectLeft, 1);
    rect1->addWidget(new QLabel("Y:"));
    rect1->addWidget(m_rectTop, 1);
    m_rectWidth = new QDoubleSpinBox();
    m_rectWidth->setDecimals(0);
    m_rectWidth->setRange(0, 10000);
    m_rectHeight = new QDoubleSpinBox();
    m_rectHeight->setDecimals(0);
    m_rectHeight->setRange(0, 10000);
    QHBoxLayout * rect2 = new QHBoxLayout();
    rect2->addWidget(new QLabel("W:"));
    rect2->addWidget(m_rectWidth, 1);
    rect2->addWidget(new QLabel("H:"));
    rect2->addWidget(m_rectHeight, 1);

    m_timeWidget = new QDoubleSpinBox();
    m_timeWidget->setSingleStep(0.05);
    QHBoxLayout * timeLayout = new QHBoxLayout();
    timeLayout->addWidget(new QLabel("Time :"));
    timeLayout->addWidget(m_timeWidget, 1);

    m_offsetX = new QDoubleSpinBox();
    m_offsetX->setDecimals(1);
    m_offsetX->setRange(-1000, 1000);
    m_offsetY = new QDoubleSpinBox();
    m_offsetY->setDecimals(1);
    m_offsetY->setRange(-1000, 1000);
    QHBoxLayout * offsetLayout = new QHBoxLayout();
    offsetLayout->addWidget(new QLabel("X: "));
    offsetLayout->addWidget(m_offsetX, 1);
    offsetLayout->addWidget(new QLabel("Y:"));
    offsetLayout->addWidget(m_offsetY, 1);

    QGroupBox * frame = new QGroupBox("Frame");
    QVBoxLayout * frameLayout = new QVBoxLayout();
    frameLayout->addLayout(timeLayout);
    frameLayout->addSpacing(5);
    frameLayout->addWidget(new QLabel("Rect"));
    frameLayout->addLayout(rect1);
    frameLayout->addLayout(rect2);
    frameLayout->addSpacing(5);
    frameLayout->addWidget(new QLabel("Offset"));
    frameLayout->addLayout(offsetLayout);
    frame->setLayout(frameLayout);


    QVBoxLayout * layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addLayout(textureLayout);
    layout->addWidget(new QLabel("Animations"));
    layout->addWidget(m_animationList, 1);
    layout->addSpacing(5);
    layout->addWidget(new QLabel("Frames"));
    layout->addWidget(m_frameList, 1);
    layout->addSpacing(5);
    layout->addWidget(frame);
    layout->addWidget(m_single);

    setLayout(layout);

    updateAnimationList();

    connect(m_chooseTextureButton, SIGNAL(clicked(bool)), this, SLOT(onSelectTexture()));
    connect(m_animationList, SIGNAL(currentRowChanged(int)), this, SLOT(onAnimationListIndexChange(int)));
    connect(m_frameList, SIGNAL(currentRowChanged(int)), this, SLOT(onFrameListIndexChange(int)));
    connect(m_single, SIGNAL(toggled(bool)), this, SLOT(onSingleShootChange()));
    connect(m_timeWidget, SIGNAL(valueChanged(double)), this, SLOT(onFrameValueChange()));
    connect(m_rectWidth, SIGNAL(valueChanged(double)), this, SLOT(onFrameValueChange()));
    connect(m_rectHeight, SIGNAL(valueChanged(double)), this, SLOT(onFrameValueChange()));
    connect(m_rectLeft, SIGNAL(valueChanged(double)), this, SLOT(onFrameValueChange()));
    connect(m_rectTop, SIGNAL(valueChanged(double)), this, SLOT(onFrameValueChange()));
    connect(m_offsetX, SIGNAL(valueChanged(double)), this, SLOT(onFrameValueChange()));
    connect(m_offsetY, SIGNAL(valueChanged(double)), this, SLOT(onFrameValueChange()));

    m_animationList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_animationList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClickAnimations(QPoint)));
    m_frameList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_frameList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClickFrames(QPoint)));
}

void AnimationList::setAll(const std::vector<Animation> & anims)
{
    m_animations = anims;
    updateAnimationList();
}

void AnimationList::frameChanged(unsigned int frameID, Frame f)
{
    auto index(currentAnimationIndex());
    if(index < 0 || index >= int(m_animations.size()))
        return;
    if(frameID >= m_animations[index].frames.size())
        return;
    m_animations[index].frames[frameID] = f;
    if(int(frameID) == currentFrameIndex())
        updateFrameData(index, frameID);
}

int AnimationList::currentAnimationIndex() const
{
    return m_animationList->currentRow();
}

int AnimationList::currentFrameIndex() const
{
    return m_frameList->currentRow();
}

void AnimationList::updateFrameData(int animationID, int frameID)
{
    m_timeWidget->blockSignals(true);
    m_rectWidth->blockSignals(true);
    m_rectHeight->blockSignals(true);
    m_rectLeft->blockSignals(true);
    m_rectTop->blockSignals(true);
    m_offsetX->blockSignals(true);
    m_offsetY->blockSignals(true);

    bool enable(animationID >= 0 && frameID >= 0);
    m_timeWidget->setEnabled(enable);
    m_rectWidth->setEnabled(enable);
    m_rectHeight->setEnabled(enable);
    m_rectLeft->setEnabled(enable);
    m_rectTop->setEnabled(enable);
    m_offsetX->setEnabled(enable);
    m_offsetY->setEnabled(enable);

    if(enable)
    {
        const Frame & f(m_animations[animationID].frames[frameID]);
        m_timeWidget->setValue(f.time);
        m_rectWidth->setValue(f.texRect.width);
        m_rectHeight->setValue(f.texRect.height);
        m_rectLeft->setValue(f.texRect.left);
        m_rectTop->setValue(f.texRect.top);
        m_offsetX->setValue(f.offset.x);
        m_offsetY->setValue(f.offset.y);
    }

    m_timeWidget->blockSignals(false);
    m_rectWidth->blockSignals(false);
    m_rectHeight->blockSignals(false);
    m_rectLeft->blockSignals(false);
    m_rectTop->blockSignals(false);
    m_offsetX->blockSignals(false);
    m_offsetY->blockSignals(false);
}

void AnimationList::updateAnimationList()
{
    m_animationList->blockSignals(true);

    m_animationList->clear();
    for(const auto & a : m_animations)
        m_animationList->addItem(QString::fromStdString(a.name));

    if(!m_animations.empty())
        m_animationList->setCurrentRow(0);

    updateFrameList(false);
    updateSingleShoot();

    auto index(currentAnimationIndex());
    if(index >= 0 && index <= int(m_animations.size()))
        emit changeAnimation(m_animations[index]);

    m_animationList->blockSignals(false);
}

void AnimationList::updateFrameList(bool notChangeIndex)
{
    m_frameList->blockSignals(true);

    int cursorIndex = m_frameList->currentRow();

    m_frameList->clear();
    auto index(currentAnimationIndex());
    if(index >= 0 && index <int(m_animations.size()))
    {
        for(const Frame & f : m_animations[index].frames)
            m_frameList->addItem("Frame " + QString::number(m_frameList->count() + 1) + " - " + QString::number(f.time));

        if(!m_animations[index].frames.empty())
            m_frameList->setCurrentRow(0);
    }

    if(notChangeIndex)
        m_frameList->setCurrentRow(cursorIndex);

    updateFrameData(currentAnimationIndex(), currentFrameIndex());

    m_frameList->blockSignals(false);
}

void AnimationList::updateSingleShoot()
{
    m_single->blockSignals(true);

   auto index(currentAnimationIndex());
   bool enable = index >= 0 && index < int(m_animations.size());
   m_single->setEnabled(enable);
   if(enable)
       m_single->setChecked(m_animations[index].singleShoot);

    m_single->blockSignals(false);
}

void AnimationList::onFrameValueChange()
{
    auto animID(currentAnimationIndex());
    auto frameID(currentFrameIndex());
    if(animID < 0 || animID >= int(m_animations.size()))
        return;
    if(frameID < 0 || frameID >= int(m_animations[animID].frames.size()))
        return;

    Frame & f(m_animations[animID].frames[frameID]);
    f.time = m_timeWidget->value();
    f.texRect = sf::FloatRect(m_rectLeft->value(), m_rectTop->value(), m_rectWidth->value(), m_rectHeight->value());
    f.offset = sf::Vector2f(m_offsetX->value(), m_offsetY->value());

    emit changeAnimation(m_animations[animID]);
    updateFrameList(true);
}

void AnimationList::onAnimationListIndexChange(int)
{
    updateSingleShoot();
    updateFrameList(false);
}

void AnimationList::onFrameListIndexChange(int newIndex)
{
    updateFrameData(currentAnimationIndex(), newIndex);
}

void AnimationList::onSingleShootChange()
{
    auto index(currentAnimationIndex());
    if(index < 0 || index >= int(m_animations.size()))
        return;
    m_animations[index].singleShoot = m_single->isChecked();

    emit changeAnimation(m_animations[index]);
}

void AnimationList::onSelectTexture()
{
    QString name = QFileDialog::getOpenFileName(this, "Ouvrir une texture ...", "", "Images (*.png *.xpm *.jpg)");
    if(name.isEmpty())
        return;

    m_texture.load(name.toStdString());
    m_textureText->setText(name);
    emit changeTexture(m_texture);
}

void AnimationList::onRightClickAnimations(QPoint point)
{
    QPoint globalPos(m_animationList->viewport()->mapToGlobal(point));

    QAction *aAdd(nullptr), *aDel(nullptr), *aChange(nullptr), *aExport(nullptr), *aImport(nullptr);
    QMenu menu;
    aAdd = menu.addAction("Ajouter une animation");

    auto index(currentAnimationIndex());
    if(index >= 0 && index < int(m_animations.size()))
    {
        aDel = menu.addAction("Suprimer l'animation");
        aChange = menu.addAction("Renommer l'animation");
        aExport = menu.addAction("Exporter l'animation");
    }

    aImport = menu.addAction("Importer");

    QAction* action = menu.exec(globalPos);
    if(action == nullptr)
        return;

    if(action == aAdd)
    {
        bool ok = false;
        QString name(QInputDialog::getText(this, "Ajouter une animation", "Nom de la nouvelle animation", QLineEdit::Normal, "", &ok));
        if(!ok || name.isEmpty())
            return;
        m_animations.push_back(Animation());
        m_animations.back().name = name.toStdString();
        updateAnimationList();
        m_animationList->setCurrentRow(m_animations.size()-1);
    }
    else if(action == aDel)
    {
        if(index < 0 || index >= int(m_animations.size()))
            return;
        int result(QMessageBox::question(this, "Supprimer", "Etes vous sur de vouloir supprimer l'animation " + QString::fromStdString(m_animations[index].name) + " ?", QMessageBox::Yes, QMessageBox::No));
        if(result != QMessageBox::Yes)
            return;
        m_animations.erase(std::next(m_animations.begin(), index));
        updateAnimationList();

    }
    else if(action == aChange)
    {
        if(index < 0 || index >= int(m_animations.size()))
            return;
        bool ok = false;
        QString name(QInputDialog::getText(this, "Ajouter une animation", "Nom de la nouvelle animation", QLineEdit::Normal, "", &ok));
        if(!ok || name.isEmpty())
            return;
        m_animations[index].name = name.toStdString();
        updateAnimationList();
    }
    else if(action == aExport)
    {
        if(index < 0 || index >= int(m_animations.size()))
            return;
        QString name = QFileDialog::getSaveFileName(this, "Sauvegarder l'animation " + QString::fromStdString(m_animations[index].name));
        if(name.isEmpty())
            return;
        if(!name.endsWith(".json"))
            name.append(".json");

        save(name, QJsonDocument(toJson(m_animations[index])));
    }
    else if(action == aImport)
    {
        QString name = QFileDialog::getOpenFileName(this, "Importer une animation");
        if(name.isEmpty())
            return;
        QJsonDocument doc = load(name);
        if(!doc.isObject())
            return;
        m_animations.push_back(toAnimation(doc.object()));
        updateAnimationList();
    }
}

void AnimationList::onRightClickFrames(QPoint point)
{
    auto indexAnimation = currentAnimationIndex();
    if(indexAnimation < 0 || indexAnimation >= int(m_animations.size()))
        return;
    Animation & anim(m_animations[indexAnimation]);

    QPoint globalPos(m_frameList->viewport()->mapToGlobal(point));

    QAction *aAdd(nullptr), *aDel(nullptr), *aUp(nullptr), *aDown(nullptr), *aDuplique(nullptr);
    QMenu menu;
    aAdd = menu.addAction("Ajouter une Frame");

    auto index(currentFrameIndex());
    if(index >= 0 && index < int(anim.frames.size()))
    {
        aDel = menu.addAction("Suprimer la frame");
        if(index >= 1)
            aUp = menu.addAction("Monter");
        if(index < int(anim.frames.size() - 1))
            aDown = menu.addAction("Descendre");
        aDuplique = menu.addAction("Dupliquer");
    }

    QAction* action = menu.exec(globalPos);
    if(action == nullptr)
        return;

    if(action == aAdd)
    {
        anim.frames.push_back(Frame(0.1, sf::FloatRect(), sf::Vector2f()));
        updateFrameList(true);
    }
    else if(action == aDel)
    {
        if(index < 0 || index >= int(anim.frames.size()))
            return;
        anim.frames.erase(std::next(anim.frames.begin(), index));
        updateFrameList(false);
    }
    else if(action == aUp)
    {
        if(index < 1 || index >= int(anim.frames.size()))
            return;
        std::swap(anim.frames[index], anim.frames[index - 1]);
        updateFrameList(true);
        m_frameList->setCurrentRow(index-1);
    }
    else if(action == aDown)
    {
        if(index < 0 || index >= int(anim.frames.size()-1))
            return;
        std::swap(anim.frames[index], anim.frames[index + 1]);
        updateFrameList(true);
        m_frameList->setCurrentRow(index+1);
    }
    else if(action == aDuplique)
    {
        if(index < 0 || index >= int(anim.frames.size()))
            return;
        anim.frames.push_back(anim.frames[index]);
        updateFrameList(false);
        m_frameList->setCurrentRow(anim.frames.size() - 1);
    }

    emit changeAnimation(anim);
}
