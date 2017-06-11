#ifndef LIGHTSDOCK_H
#define LIGHTSDOCK_H

#include "unclosabledock.h"
#include "Map/patern.h"
#include "Gui/lightholder.h"
#include <QListWidget>
#include <QFrame>
#include <QPushButton>
#include <memory>

class LightsDock : public UnclosableDock
{
    Q_OBJECT
public:
    LightsDock(QWidget * parent = nullptr);

public slots:
    void changeRoom(std::weak_ptr<Patern> room);
    void onIndexChange(int index);
    void onAddClicked();
    void onDelClicked();

private:
    void updateButtonsVisibility();
    void updateLightList();

    std::weak_ptr<Patern> m_room;

    QListWidget *m_lights;
    QFrame *m_lightFrame;
    LightHolder *m_lightHolder;
    QPushButton *m_addButton;
    QPushButton *m_delButton;
};

#endif // LIGHTSDOCK_H
