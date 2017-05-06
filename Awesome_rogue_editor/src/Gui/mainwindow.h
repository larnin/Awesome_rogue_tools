#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "centralview.h"
#include "Dock/blockdock.h"
#include "Dock/paternsdock.h"
#include "Map/patern.h"
#include <QMainWindow>
#include <vector>
#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() = default;

public slots:
    void openSlot();
    void saveSlot();
    void saveAsSlot();
    void importSlot();
    void closeSlot();
    void quitSlot();

    void addPaternSlot();
    void delPaternSlot(int id);
    void updatePaternSlot(int id, PaternInfo infos);
    void movePaternSlot(int oldId, int newId);
    void dupliquePaternSlot(int id);
    void selectPaternSlot(int id);

    void selectGridColor();

    void returnAddPaternSlot(PaternInfo infos);

signals:

private:
    void compilePaternsInfos();

    CentralView *m_view;
    BlockDock *m_blocks;
    PaternsDock *m_paternsInfos;

    std::vector<std::shared_ptr<Patern>> m_paterns;
    std::string m_fileName;
};

#endif // MAINWINDOW_H
