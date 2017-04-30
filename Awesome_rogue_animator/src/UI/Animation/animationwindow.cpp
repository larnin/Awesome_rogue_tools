#include "animationwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

AnimationWindow::AnimationWindow(QWidget *parent)
    : QWidget(parent)
{
    m_animationList = new AnimationList();
    m_animationPreview = new AnimationPreview();
    m_animationTextureView = new AnimationTextureView();

    QWidget * verticalLine = new QWidget();
    verticalLine->setFixedWidth(1);
    verticalLine->setStyleSheet(QString("background-color: #c0c0c0;"));

    QGroupBox * previewBox = new QGroupBox("Preview");
    QVBoxLayout * previewLayout = new QVBoxLayout();
    previewLayout->addWidget(m_animationPreview);
    previewBox->setLayout(previewLayout);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(m_animationList, 4);
    layout->addWidget(previewBox, 1);

    QHBoxLayout * centralLayout = new QHBoxLayout();
    centralLayout->addWidget(m_animationTextureView, 1);
    centralLayout->addWidget(verticalLine);
    centralLayout->addLayout(layout);

    setLayout(centralLayout);

    connect(m_animationList, SIGNAL(changeAnimation(Animation)), m_animationPreview, SLOT(onAnimationChange(Animation)));
    connect(m_animationList, SIGNAL(changeTexture(Texture)), m_animationPreview, SLOT(onTextureChange(Texture)));
    connect(m_animationList, SIGNAL(changeAnimation(Animation)), m_animationTextureView, SLOT(onAnimationChange(Animation)));
    connect(m_animationList, SIGNAL(changeTexture(Texture)), m_animationTextureView, SLOT(onTextureChange(Texture)));
    connect(m_animationTextureView, SIGNAL(changeFrame(unsigned int, Frame)), m_animationList, SLOT(frameChanged(unsigned int,Frame)));
    connect(m_animationList, SIGNAL(animationAdded()), this, SIGNAL(animationAdded()));
    connect(m_animationList, SIGNAL(animationDeleted(uint)), this, SIGNAL(animationDeleted(uint)));
    connect(m_animationList, SIGNAL(animationRenamed(uint)), this, SIGNAL(animationRenamed(uint)));
}

void AnimationWindow::onAnimationsSet()
{
    m_animationList->onAnimationsSet();
}

void AnimationWindow::setTexture(Texture t)
{
    m_animationList->onTextureLoad(t);
    m_animationPreview->onTextureChange(t);
    m_animationTextureView->onTextureChange(t);
}


void AnimationWindow::setColor(sf::Color c)
{
    m_animationTextureView->setBackgroundColor(c);
    m_animationPreview->setBackgroundColor(c);
}
