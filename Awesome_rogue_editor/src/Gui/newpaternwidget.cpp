#include "newpaternwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

NewPaternWidget::NewPaternWidget(QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_valid = new QPushButton("Valider");
    m_annule = new QPushButton("Annuler");
    m_name = new QLineEdit();
    m_sizeX = new QSpinBox();
    m_sizeX->setRange(7, 200);
    m_sizeY = new QSpinBox();
    m_sizeY->setRange(7, 200);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addStretch(1);
    buttons->addWidget(m_valid);
    buttons->addWidget(m_annule);

    QHBoxLayout *sizeButtons = new QHBoxLayout();
    sizeButtons->addWidget(new QLabel("X:"));
    sizeButtons->addWidget(m_sizeX, 1);
    sizeButtons->addSpacing(10);
    sizeButtons->addWidget(new QLabel("Y:"));
    sizeButtons->addWidget(m_sizeY, 1);

    QVBoxLayout *principalLayout = new QVBoxLayout();
    principalLayout->addWidget(new QLabel("Nom du patern :"));
    principalLayout->addWidget(m_name);
    principalLayout->addSpacing(5);
    principalLayout->addWidget(new QLabel("Taille :"));
    principalLayout->addLayout(sizeButtons);
    principalLayout->addSpacing(10);
    principalLayout->addLayout(buttons);

    setFixedSize(350, 150);

    setLayout(principalLayout);

    connect(m_valid, SIGNAL(clicked()), this, SLOT(validClicked()));
    connect(m_annule, SIGNAL(clicked()), this, SLOT(close()));
}

void NewPaternWidget::validClicked()
{
    emit valid(PaternInfo(m_name->text().toStdString(), sf::Vector2u(m_sizeX->value(), m_sizeY->value()), Patern::RoomType::NORMAL_ROOM, 50));
    close();
}
