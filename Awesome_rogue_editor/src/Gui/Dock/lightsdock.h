#ifndef LIGHTSDOCK_H
#define LIGHTSDOCK_H

#include "unclosabledock.h"
#include "Map/patern.h"
#include "Gui/lightholder.h"
#include <QListWidget>
#include <QFrame>
#include <QPushButton>
#include <QDoubleSpinBox>
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
    void onLightChange();
    void onAmbiantClicked();

signals:
    void ambiantChange(float);

private:
    void updateButtonsVisibility();
    void updateLightList();
    void softUpdateLightList();

    std::weak_ptr<Patern> m_room;

    QListWidget *m_lights;
    QFrame *m_lightFrame;
    LightHolder *m_lightHolder;
    QPushButton *m_addButton;
    QPushButton *m_delButton;
    QPushButton *m_ambiantWidget;
};

#endif // LIGHTSDOCK_H
