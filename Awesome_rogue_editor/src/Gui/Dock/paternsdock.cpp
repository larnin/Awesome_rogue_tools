#include "paternsdock.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QFrame>


PaternsDock::PaternsDock(QWidget * parent)
    : UnclosableDock("Paterns", parent)
{
    m_paternsWidget = new QListWidget();

    m_addWidget = new QPushButton("Ajouter");
    m_delWidget = new QPushButton("Suprimer");
    m_delWidget->setDisabled(true);
    m_dupliqueWidget = new QPushButton("Dupliquer");
    m_dupliqueWidget->setDisabled(true);
    QHBoxLayout *buttons1 = new QHBoxLayout();
    buttons1->addWidget(m_addWidget);
    buttons1->addWidget(m_delWidget);
    buttons1->addWidget(m_dupliqueWidget);

    m_upWidget = new QPushButton("Monter");
    m_upWidget->setDisabled(true);
    m_downWidget = new QPushButton("Descendre");
    m_downWidget->setDisabled(true);
    QHBoxLayout *buttons2 = new QHBoxLayout();
    buttons2->addWidget(m_upWidget);
    buttons2->addWidget(m_downWidget);

    m_nameWidget = new QLineEdit();
    m_rarityWidget = new QSpinBox();
    m_rarityWidget->setMinimum(1);
    m_rarityWidget->setMaximum(1000);
    m_rarityWidget->setValue(50);

    m_typeRoom = new QComboBox();
    m_typeRoom->addItem("Normale");
    m_typeRoom->addItem("Depart");
    m_typeRoom->addItem("Boss 1");
    m_typeRoom->addItem("Boss 2");
    m_typeRoom->addItem("Boss 3");
    m_typeRoom->addItem("Boss 4");
    m_typeRoom->addItem("Boss Final");

    m_sizeXWidget = new QSpinBox();
    m_sizeXWidget->setMaximum(200);
    m_sizeXWidget->setMinimum(7);
    m_sizeYWidget = new QSpinBox();
    m_sizeYWidget->setMaximum(200);
    m_sizeYWidget->setMinimum(7);
    QHBoxLayout *sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(new QLabel("X :"));
    sizeLayout->addWidget(m_sizeXWidget, 1);
    sizeLayout->addSpacing(20);
    sizeLayout->addWidget(new QLabel("Y :"));
    sizeLayout->addWidget(m_sizeYWidget, 1);
    QGroupBox *sizeBox = new QGroupBox("Taille :");
    sizeBox->setLayout(sizeLayout);

    m_paternFrame = new QFrame();
    m_paternFrame->setFrameShape(QFrame::StyledPanel);
    m_paternFrame->setDisabled(true);
    QVBoxLayout *paternLayout = new QVBoxLayout();
    paternLayout->addWidget(new QLabel("Nom :"));
    paternLayout->addWidget(m_nameWidget);
    paternLayout->addWidget(new QLabel("Type de salle :"));
    paternLayout->addWidget(m_typeRoom);
    paternLayout->addWidget(sizeBox);
    paternLayout->addWidget(new QLabel("Raretée :"));
    paternLayout->addWidget(m_rarityWidget);
    m_paternFrame->setLayout(paternLayout);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Paterns :"));
    layout->addWidget(m_paternsWidget);
    layout->addLayout(buttons1);
    layout->addLayout(buttons2);
    layout->addWidget(m_paternFrame);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setWidget(widget);

    connect(m_addWidget, SIGNAL(clicked()), this, SIGNAL(addPatern()));
    connect(m_delWidget, SIGNAL(clicked()), this, SLOT(delClicked()));
    connect(m_upWidget, SIGNAL(clicked()), this, SLOT(upClicked()));
    connect(m_downWidget, SIGNAL(clicked()), this, SLOT(downClicked()));
    connect(m_dupliqueWidget, SIGNAL(clicked()), this, SLOT(dupliqueClicked()));

    connect(m_sizeXWidget, SIGNAL(editingFinished()), this, SLOT(caracteristiqueChanged()));
    connect(m_sizeYWidget, SIGNAL(editingFinished()), this, SLOT(caracteristiqueChanged()));
    connect(m_nameWidget, SIGNAL(editingFinished()), this, SLOT(caracteristiqueChanged()));
    connect(m_typeRoom, SIGNAL(currentIndexChanged(int)), this, SLOT(caracteristiqueChanged()));
    connect(m_rarityWidget, SIGNAL(editingFinished()), this, SLOT(caracteristiqueChanged()));

    connect(m_paternsWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectedChanged(QModelIndex)));
}

void PaternsDock::updatePaternList(std::vector<PaternInfo> paterns)
{
    int actualIndex(m_paternsWidget->currentRow());
    m_paternsWidget->clear();
    m_paternsInfo = paterns;
    for(const PaternInfo & p : paterns)
        m_paternsWidget->addItem(QString::fromStdString(p.name) + " [" + QString::number(p.size.x) + " " + QString::number(p.size.y) + "]");

    if(m_paternsWidget->count() == 0)
        m_paternsWidget->setCurrentRow(-1);
    else if(m_paternsWidget->count() <= actualIndex)
        m_paternsWidget->setCurrentRow(m_paternsWidget->count()-1);
    else m_paternsWidget->setCurrentRow(actualIndex);
}

void PaternsDock::delClicked()
{
    int row(m_paternsWidget->currentRow());
    if(row >= m_paternsWidget->count() || row < 0 || row >= int(m_paternsInfo.size()))
        return;
    selectedChanged(-1);
    emit delPatern(row);
}

void PaternsDock::upClicked()
{
    int row(m_paternsWidget->currentRow());
    if(row >= m_paternsWidget->count() || row < 1 || row >= int(m_paternsInfo.size()))
        return;
    emit movePatern(row, row-1);
}

void PaternsDock::downClicked()
{
    int row(m_paternsWidget->currentRow());
    if(row >= m_paternsWidget->count()-1 || row < 0 || row >= int(m_paternsInfo.size())-1)
        return;
    emit movePatern(row, row+1);
}

void PaternsDock::dupliqueClicked()
{
    int row(m_paternsWidget->currentRow());
    if(row >= m_paternsWidget->count() || row < 0 || row >= int(m_paternsInfo.size()))
        return;
    emit dupliquePatern(row);
}

void PaternsDock::caracteristiqueChanged()
{
    int row(m_paternsWidget->currentRow());
    if(row >= m_paternsWidget->count() || row < 0 || row >= int(m_paternsInfo.size()))
        return;
    Patern::RoomType type;
    if(m_typeRoom->currentIndex() == 0)
        type = Patern::RoomType::NORMAL_ROOM;
    else if(m_typeRoom->currentIndex() == 1)
        type = Patern::RoomType::START_ROOM;
    else if(m_typeRoom->currentIndex() == 2)
        type = Patern::RoomType::BOSS1_ROOM;
    else if(m_typeRoom->currentIndex() == 3)
        type = Patern::RoomType::BOSS2_ROOM;
    else if(m_typeRoom->currentIndex() == 4)
        type = Patern::RoomType::BOSS3_ROOM;
    else if(m_typeRoom->currentIndex() == 5)
        type = Patern::RoomType::BOSS4_ROOM;
    else type = Patern::RoomType::FINAL_ROOM;

    PaternInfo i(m_nameWidget->text().toStdString(), sf::Vector2u(m_sizeXWidget->value(), m_sizeYWidget->value()), type, m_rarityWidget->value());
    emit updatePatern(row, i);
}


void PaternsDock::selectedChanged(const QModelIndex & index)
{
    selectedChanged(index.row());

}

void PaternsDock::selectedChanged(int row)
{
    m_paternsWidget->blockSignals(true);
    if(row != m_paternsWidget->currentRow())
        m_paternsWidget->setCurrentRow(row);
    m_paternsWidget->blockSignals(false);

    if(row >= m_paternsWidget->count() || row < 0 || row >= int(m_paternsInfo.size()))
    {
        m_paternFrame->setDisabled(true);
        m_delWidget->setDisabled(true);
        m_upWidget->setDisabled(true);
        m_downWidget->setDisabled(true);
        m_dupliqueWidget->setDisabled(true);
        return;
    }

    m_paternFrame->setEnabled(true);
    m_delWidget->setEnabled(true);
    m_dupliqueWidget->setEnabled(true);

    if(row == 0)
        m_upWidget->setDisabled(true);
    else m_upWidget->setEnabled(true);
    if(row == m_paternsWidget->count()-1)
        m_downWidget->setDisabled(true);
    else m_downWidget->setEnabled(true);

    const PaternInfo & patern(m_paternsInfo[row]);

    m_sizeXWidget->blockSignals(true);
    m_sizeYWidget->blockSignals(true);
    m_nameWidget->blockSignals(true);
    m_typeRoom->blockSignals(true);
    m_rarityWidget->blockSignals(true);

    m_sizeXWidget->setValue(patern.size.x);
    m_sizeYWidget->setValue(patern.size.y);
    m_nameWidget->setText(QString::fromStdString(patern.name));
    if(patern.type == Patern::RoomType::NORMAL_ROOM)
        m_typeRoom->setCurrentIndex(0);
    else if(patern.type == Patern::RoomType::START_ROOM)
        m_typeRoom->setCurrentIndex(1);
    else m_typeRoom->setCurrentIndex(2);
    m_rarityWidget->setValue(patern.rarity);

    m_sizeXWidget->blockSignals(false);
    m_sizeYWidget->blockSignals(false);
    m_nameWidget->blockSignals(false);
    m_typeRoom->blockSignals(false);
    m_rarityWidget->blockSignals(false);

    emit selectPatern(row);
}
