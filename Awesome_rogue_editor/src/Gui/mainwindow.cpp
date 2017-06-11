#include "mainwindow.h"
#include "newpaternwidget.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QColorDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_blocks = new BlockDock();
    addDockWidget(Qt::LeftDockWidgetArea, m_blocks);

    m_paternsInfos = new PaternsDock();
    addDockWidget(Qt::RightDockWidgetArea, m_paternsInfos);

    m_lights = new LightsDock();
    addDockWidget(Qt::LeftDockWidgetArea, m_lights);
    tabifyDockWidget(m_blocks, m_lights);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
    m_blocks->raise();

    m_view = new CentralView();
    m_view->setMinimumWidth(200);
    setCentralWidget(m_view);

    QMenu* fileMenu = menuBar()->addMenu("&Fichier");
        QAction* aOpen = fileMenu->addAction("&Ouvrir");
            aOpen->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_0));
        QAction* aSave = fileMenu->addAction("&Sauvegarder");
            aSave->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
        QAction* aSaveAs = fileMenu->addAction("Sauvegarder sous ...");
        QAction* aImport = fileMenu->addAction("&Importer");
        fileMenu->addSeparator();
        QAction* aClose = fileMenu->addAction("&Close");
        QAction* aSaveQuit = fileMenu->addAction("Sauvegarder && &Quitter");

    QMenu* editMenu = menuBar()->addMenu("&Edition");
        QAction* aUndo = editMenu->addAction("Undo");
            aUndo->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
        QAction *aRedo = editMenu->addAction("Redo");
            aRedo->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Y));
        editMenu->addSeparator();
        QAction* aCopy = editMenu->addAction("Copier");
            aCopy->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
        QAction* aPast = editMenu->addAction("Coller");
            aPast->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_V));
        editMenu->addSeparator();
        QAction* aSelectMode = editMenu->addAction("Mode sélection");
            aSelectMode->setShortcut(QKeySequence(Qt::Key_Tab));
            aSelectMode->setCheckable(true);
        QAction* aDrawWall = editMenu->addAction("Change le mur");
            aDrawWall->setCheckable(true);
            aDrawWall->setChecked(true);
        QAction* aDrawGround = editMenu->addAction("Change le sol");
            aDrawGround->setCheckable(true);
            aDrawGround->setChecked(true);
        QAction* aDrawTop = editMenu->addAction("Change le plafond");
            aDrawTop->setCheckable(true);
            aDrawTop->setChecked(true);

    QMenu* viewMenu = menuBar()->addMenu("&Vues");
        QAction* aShowGrid = viewMenu->addAction("Afficher la grille");
            aShowGrid->setCheckable(true);
        QAction* aColorGrid = viewMenu->addAction("Couleur de la grille");
        QAction* aShowHitbox = viewMenu->addAction("Afficher les murs");
            aShowHitbox->setCheckable(true);

    connect(aOpen, SIGNAL(triggered()), this, SLOT(openSlot()));
    connect(aSave, SIGNAL(triggered()), this, SLOT(saveSlot()));
    connect(aSaveAs, SIGNAL(triggered()), this, SLOT(saveAsSlot()));
    connect(aImport, SIGNAL(triggered()), this, SLOT(importSlot()));
    connect(aClose, SIGNAL(triggered()), this, SLOT(closeSlot()));
    connect(aSaveQuit, SIGNAL(triggered()), this, SLOT(quitSlot()));

    connect(aUndo, SIGNAL(triggered()), m_view, SLOT(undo()));
    connect(aRedo, SIGNAL(triggered()), m_view, SLOT(redo()));
    connect(aSelectMode, SIGNAL(triggered(bool)), m_view, SLOT(switchSelectionMode(bool)));
    connect(aCopy, SIGNAL(triggered()), m_view, SLOT(copy()));
    connect(aPast, SIGNAL(triggered()), m_view, SLOT(past()));

    connect(aDrawWall, SIGNAL(triggered(bool)), m_view, SLOT(setDrawWall(bool)));
    connect(aDrawGround, SIGNAL(triggered(bool)), m_view, SLOT(setDrawGround(bool)));
    connect(aDrawTop, SIGNAL(triggered(bool)), m_view, SLOT(setDrawTop(bool)));
    connect(aShowGrid, SIGNAL(triggered(bool)), m_view, SLOT(setGridShow(bool)));
    connect(aColorGrid, SIGNAL(triggered()), this, SLOT(selectGridColor()));
    connect(aShowHitbox, SIGNAL(triggered(bool)), m_view, SLOT(showWallsHitbox(bool)));

    connect(m_blocks, SIGNAL(changeBlock(Block)), m_view, SLOT(setSelectedBlock(Block)));
    connect(m_view, SIGNAL(sendPickedBlock(Block)), m_blocks, SLOT(blockPicked(Block)));

    connect(m_paternsInfos, SIGNAL(addPatern()), this, SLOT(addPaternSlot()));
    connect(m_paternsInfos, SIGNAL(delPatern(int)), this, SLOT(delPaternSlot(int)));
    connect(m_paternsInfos, SIGNAL(updatePatern(int,PaternInfo)), this, SLOT(updatePaternSlot(int,PaternInfo)));
    connect(m_paternsInfos, SIGNAL(movePatern(int,int)), this, SLOT(movePaternSlot(int,int)));
    connect(m_paternsInfos, SIGNAL(dupliquePatern(int)), this, SLOT(dupliquePaternSlot(int)));
    connect(m_paternsInfos, SIGNAL(selectPatern(int)), this, SLOT(selectPaternSlot(int)));

    connect(m_blocks, SIGNAL(tileConfigChanged()), m_view, SLOT(onTileConfigChanged()));
}

void MainWindow::openSlot()
{
    QString fileName(QFileDialog::getOpenFileName(this, "Ouvrir ...", QString(), "*.json"));
    if(fileName.isEmpty())
        return;
    m_paterns = Patern::load(fileName.toStdString());

    m_paternsInfos->updatePaternList(std::vector<PaternInfo>());
    m_view->setPatern(std::weak_ptr<Patern>());
    compilePaternsInfos();
    m_fileName = fileName.toStdString();
}

void MainWindow::saveSlot()
{
    if(m_fileName.empty())
    {
        saveAsSlot();
        return;
    }
    Patern::save(m_fileName, m_paterns);
}

void MainWindow::saveAsSlot()
{
    QString file(QFileDialog::getSaveFileName(this, "Sauvegarder ...", QString(), "*.json"));
       if(file.isEmpty())
           return;
    if(!file.endsWith(".json"))
        file+=".json";
    m_fileName = file.toStdString();

    saveSlot();
}

void MainWindow::importSlot()
{
    QString fileName(QFileDialog::getOpenFileName(this, "Ouvrir ...", QString(), "*.json"));
    if(fileName.isEmpty())
        return;
    auto paterns(Patern::load(fileName.toStdString()));
    for(auto p : paterns)
        m_paterns.push_back(p);
    compilePaternsInfos();
}

void MainWindow::closeSlot()
{
    auto anser(QMessageBox::question(this, "Fermer", "Vraiment fermer sans sauvegarder ?", QMessageBox::Yes | QMessageBox::No));
    if(anser == QMessageBox::Yes)

    m_paterns.clear();
    m_view->setPatern(std::weak_ptr<Patern>());
    m_lights->changeRoom(std::weak_ptr<Patern>());
    compilePaternsInfos();
    m_fileName.clear();
}

void MainWindow::quitSlot()
{
    saveSlot();
    close();
}

void MainWindow::addPaternSlot()
{
    NewPaternWidget *w = new NewPaternWidget();
    connect(w, SIGNAL(valid(PaternInfo)), this, SLOT(returnAddPaternSlot(PaternInfo)));
    w->show();
}

void MainWindow::delPaternSlot(int id)
{
    if(id < 0 || id >= int(m_paterns.size()))
        return;
    m_paterns.erase(m_paterns.begin() + id);
    compilePaternsInfos();

    m_lights->changeRoom(std::weak_ptr<Patern>());
}

void MainWindow::updatePaternSlot(int id, PaternInfo infos)
{
    if(id < 0 || id >= int(m_paterns.size()))
        return;

    std::shared_ptr<Patern> p(m_paterns[id]);
    if(!p)
        return;

    if(p->getSize() != infos.size)
    {
        Patern pNew(infos.size);
        sf::Vector2i delta((sf::Vector2i(p->getSize()) - sf::Vector2i(infos.size) ) / 2);

        for(unsigned int i(0) ; i < pNew.getSize().x ; i++)
            for(unsigned int j(0) ; j < pNew.getSize().y ; j++)
            {
                sf::Vector2i pos(i+delta.x, j+delta.y);
                if(pos.x < 0 || pos.y < 0 || pos.x >= int(p->getSize().x) || pos.y >= int(p->getSize().y))
                    pNew(sf::Vector2u(i, j)) = Block();
                else pNew(sf::Vector2u(i, j)) = (*p)(sf::Vector2u(pos));
            }
        *p = pNew;

        m_view->setPatern(p);
        m_lights->changeRoom(p);
    }

    p->name = infos.name;
    p->type = infos.type;
    p->rarity = infos.rarity;

    compilePaternsInfos();
}

void MainWindow::movePaternSlot(int oldId, int newId)
{
    if(oldId < 0 || oldId >= int(m_paterns.size()) || newId < 0 || newId >= int(m_paterns.size()))
        return;

    std::swap(m_paterns[oldId], m_paterns[newId]);
    compilePaternsInfos();
}

void MainWindow::dupliquePaternSlot(int id)
{
    if(id < 0 || id >= int(m_paterns.size()))
        return;

    QString name(QInputDialog::getText(this, "Dupliquer le patern" + QString::fromStdString(m_paterns[id]->name)
                          , "Entrez le nouveau nom du patern dupliqué :"));
    if(name.isEmpty())
        return;

    Patern p(*(m_paterns[id]));
    p.name = name.toStdString();
    m_paterns.push_back(std::make_shared<Patern>(p));
    compilePaternsInfos();
}

void MainWindow::selectPaternSlot(int id)
{
    if(id < 0 || id >= int(m_paterns.size()))
        return;

    m_view->setPatern(m_paterns[id]);
    m_lights->changeRoom(m_paterns[id]);
}

void MainWindow::returnAddPaternSlot(PaternInfo infos)
{
    Patern p(infos.size);
    p.name = infos.name;
    p.type = infos.type;
    p.rarity = infos.rarity;
    m_paterns.push_back(std::make_shared<Patern>(p));
    compilePaternsInfos();
}

void MainWindow::selectGridColor()
{
    QColor color(QColorDialog::getColor(Qt::white, this, "Couleur de grille", QColorDialog::ShowAlphaChannel));
    if(!color.isValid())
        return;
    m_view->setGridColor(sf::Color(color.red(), color.green(), color.blue(), color.alpha()));
}

void MainWindow::compilePaternsInfos()
{
    std::vector<PaternInfo> infos;

    for(const auto & p : m_paterns)
        infos.push_back(PaternInfo(p->name, p->getSize(), p->type, p->rarity));
    m_paternsInfos->updatePaternList(infos);
}
