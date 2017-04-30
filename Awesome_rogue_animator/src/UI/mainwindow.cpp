#include "mainwindow.h"
#include "datas.h"
#include <QMenu>
#include <QMenuBar>
#include <QMdiArea>
#include <algorithm>
#include <QColorDialog>
#include <QMessageBox>
#include <QFileDialog>

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

    QMenu * fileMenu = menuBar()->addMenu("Fichier");
    QMenu * viewMenu = menuBar()->addMenu("Vue");

    QAction * newAction = fileMenu->addAction("Nouveau");
    QAction * loadAction = fileMenu->addAction("Charger");
    QAction * saveAction = fileMenu->addAction("Savegarder");
    QAction * saveAsAction = fileMenu->addAction("Sauvegarder vers ...");
    QAction * backColorAction = viewMenu->addAction("Couleur du fond");

    connect(newAction, SIGNAL(triggered(bool)), this, SLOT(onNew()));
    connect(loadAction, SIGNAL(triggered(bool)), this, SLOT(onLoad()));
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(onSave()));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(onSaveAs()));
    connect(backColorAction, SIGNAL(triggered(bool)), this, SLOT(onBackColorChange()));

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


void MainWindow::onNew()
{
    auto value = QMessageBox::question(this, "Nouveau", "Etes-vous sur de vouloir commencer un nouvel animator ?", QMessageBox::Yes | QMessageBox::No);

    if(value != QMessageBox::Yes)
        return;

    m_filename = "";
    Datas::instance().reset();

    m_animationWindow->onAnimationsSet();
    m_animationWindow->setTexture(Datas::instance().texture);
    m_animatorWindow->onAnimationListChanged();
}

void MainWindow::onLoad()
{
    QString file = QFileDialog::getOpenFileName(this, "Charger un animator", "", "*.json");
    if(file.isEmpty())
        return;

    m_filename = file;

    Datas::instance().loadDatas(file);
    m_animationWindow->onAnimationsSet();
    m_animationWindow->setTexture(Datas::instance().texture);
    m_animatorWindow->onAnimationListChanged();
}

void MainWindow::onSave()
{
    if(m_filename.isEmpty())
    {
        onSaveAs();
        return;
    }

    Datas::instance().saveDatas(m_filename);
}

void MainWindow::onSaveAs()
{
    QString file = QFileDialog::getSaveFileName(this, "Sauvegarder", "", "*.json");
    if(file.isEmpty())
        return;

    if(!file.endsWith(".json"))
        file += ".json";
    m_filename = file;

    onSave();
}

void MainWindow::onBackColorChange()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Selectionner une couleur de fond");
    if(!color.isValid())
        return;

    m_animationWindow->setColor(sf::Color(color.red(), color.green(), color.blue()));
}
