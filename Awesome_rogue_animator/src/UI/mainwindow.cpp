#include "mainwindow.h"
#include "datas.h"
#include <QMdiArea>
#include <algorithm>

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
    connect(m_animationWindow, SIGNAL(animationDeleted(uint)), this, SLOT(onDeleteAnimation(uint)));
    connect(m_animationWindow, SIGNAL(animationRenamed(uint)), m_animatorWindow, SLOT(onAnimationListChanged()));
}

void MainWindow::onDeleteAnimation(unsigned int index)
{
    if(index < Datas::instance().size())
    {
        for(State & s : Datas::instance())
        {
            s.transitions.erase(std::remove_if(s.transitions.begin(), s.transitions.end(), [index](const auto & t){return t.targetAnimationID == index;}), s.transitions.end());
            for(Transition & t : s.transitions)
                if(t.targetAnimationID > index)
                    t.targetAnimationID--;
        }
    }

    m_animatorWindow->onAnimationListChanged();
}
