#include "animatorwindow.h"
#include <QHBoxLayout>

AnimatorWindow::AnimatorWindow(QWidget * parent)
    : QWidget(parent)
{
    m_animationList = new AnimatorAnimationList();

    QWidget * verticalLine = new QWidget();
    verticalLine->setFixedWidth(1);
    verticalLine->setStyleSheet(QString("background-color: #c0c0c0;"));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_animationList);
    layout->addWidget(verticalLine);

    setLayout(layout);
}

void AnimatorWindow::onAnimationListChanged()
{
    m_animationList->onAnimationListChanged();
}
