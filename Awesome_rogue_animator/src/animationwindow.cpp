#include "animationwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

AnimationWindow::AnimationWindow(QWidget *parent)
    : QWidget(parent)
{
    m_animationList = new AnimationList();
    m_animationPreview = new AnimationPreview();

    QGroupBox * previewBox = new QGroupBox("Preview");
    QVBoxLayout * previewLayout = new QVBoxLayout();
    previewLayout->addWidget(m_animationPreview);
    previewBox->setLayout(previewLayout);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(m_animationList, 4);
    layout->addWidget(previewBox, 1);

    setLayout(layout);

    connect(m_animationList, SIGNAL(changeAnimation(Animation)), m_animationPreview, SLOT(onAnimationChange(Animation)));
    connect(m_animationList, SIGNAL(changeTexture(Texture)), m_animationPreview, SLOT(onTextureChange(Texture)));
}
