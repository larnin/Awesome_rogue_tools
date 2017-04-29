#include "animatorwindow.h"
#include <QHBoxLayout>

AnimatorWindow::AnimatorWindow(QWidget * parent)
    : QWidget(parent)
{
    m_animationList = new AnimatorAnimationList();
    m_centralView = new AnimatorCentralView();
    m_transitionTab = new AnimatorTransitionsTab();

    QWidget * verticalLine = new QWidget();
    verticalLine->setFixedWidth(1);
    verticalLine->setStyleSheet(QString("background-color: #c0c0c0;"));
    QWidget * verticalLine2 = new QWidget();
    verticalLine2->setFixedWidth(1);
    verticalLine2->setStyleSheet(QString("background-color: #c0c0c0;"));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_animationList);
    layout->addWidget(verticalLine);
    layout->addWidget(m_centralView, 1);
    layout->addWidget(verticalLine2);
    layout->addWidget(m_transitionTab);

    setLayout(layout);

    connect(m_centralView, SIGNAL(selectedItemChanged(int)), m_animationList, SLOT(onItemSelectedChanged(int)));
    connect(m_centralView, SIGNAL(defaultStateChanged(int)), m_animationList, SLOT(onDefaultStateChanged()));

    connect(m_animationList, SIGNAL(indexChanged(int)), m_centralView, SLOT(onItemSelectedChanged(int)));

    connect(m_centralView, SIGNAL(newTransition(int)), m_transitionTab, SLOT(onAddTransition(int)));
    connect(m_centralView, SIGNAL(selectedItemChanged(int)), m_transitionTab, SLOT(onStateIndexChange(int)));
    connect(m_animationList, SIGNAL(indexChanged(int)), m_transitionTab, SLOT(onStateIndexChange(int)));
}

void AnimatorWindow::onAnimationListChanged()
{
    m_animationList->onAnimationListChanged();
    m_transitionTab->onAnimationListChanged();
}
