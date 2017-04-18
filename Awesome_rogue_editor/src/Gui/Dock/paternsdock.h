#ifndef PATERNSDOCK_H
#define PATERNSDOCK_H

#include <string>
#include <vector>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <SFML/System/Vector2.hpp>

#include "unclosabledock.h"
#include "Map/patern.h"

struct PaternInfo
{
    PaternInfo(const std::string & _name, const sf::Vector2u & _size, Patern::RoomType _type, unsigned int _rarity)
        : name(_name) , size(_size) , type(_type) , rarity(_rarity) {}

    std::string name;
    sf::Vector2u size;
    Patern::RoomType type;
    unsigned int rarity;
};

class PaternsDock : public UnclosableDock
{
    Q_OBJECT
public:
    PaternsDock(QWidget *parent = nullptr);
    virtual ~PaternsDock() = default;

public slots:
    void updatePaternList(std::vector<PaternInfo> paterns);

    void delClicked();
    void upClicked();
    void downClicked();
    void dupliqueClicked();
    void caracteristiqueChanged();
    void selectedChanged(const QModelIndex & index);

signals:
    void addPatern();
    void delPatern(int);
    void updatePatern(int, PaternInfo);
    void movePatern(int, int);
    void dupliquePatern(int);
    void selectPatern(int);

private:
    QListWidget *m_paternsWidget;
    QPushButton *m_addWidget;
    QPushButton *m_delWidget;
    QPushButton *m_upWidget;
    QPushButton *m_downWidget;
    QPushButton *m_dupliqueWidget;

    QSpinBox *m_sizeXWidget;
    QSpinBox *m_sizeYWidget;
    QLineEdit *m_nameWidget;
    QComboBox *m_typeRoom;
    QSpinBox *m_rarityWidget;

    QFrame *m_paternFrame;

    std::vector<PaternInfo> m_paternsInfo;
};

#endif // PATERNSDOCK_H
