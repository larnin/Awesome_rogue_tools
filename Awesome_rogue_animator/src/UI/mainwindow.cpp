#include "mainwindow.h"
#include <QMdiArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_animationWindow = new AnimationWindow();
    m_animationWindow->setWindowTitle("Animations");
    m_animatorWindow = new AnimatorWindow();
    m_animatorWindow->setWindowTitle("Animator");

    QMdiArea * center = new QMdiArea();
    auto w1 = center->addSubWindow(m_animationWindow);
    center->addSubWindow(m_animatorWindow);
    center->setViewMode(QMdiArea::TabbedView);
    center->setActiveSubWindow(w1);

    setCentralWidget(center);

    connect(m_animationWindow, SIGNAL(animationAdded()), m_animatorWindow, SLOT(onAnimationListChanged()));
    connect(m_animationWindow, SIGNAL(animationDeleted(uint)), m_animatorWindow, SLOT(onAnimationListChanged()));
    connect(m_animationWindow, SIGNAL(animationRenamed(uint)), m_animatorWindow, SLOT(onAnimationListChanged()));
}
