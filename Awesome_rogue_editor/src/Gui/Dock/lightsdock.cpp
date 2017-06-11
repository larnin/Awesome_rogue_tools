#include "lightsdock.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

LightsDock::LightsDock(QWidget *parent)
    : UnclosableDock("Lumieres", parent)
    , m_lightHolder(nullptr)
{
    m_lights = new QListWidget();
    m_lightFrame = new QFrame();
    m_lightFrame->setLayout(new QVBoxLayout());
    m_lightFrame->setFrameShape(QFrame::Box);

    m_addButton = new QPushButton("Ajouter");
    m_delButton = new QPushButton("Suprimer");
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_delButton);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Lumieres :"));
    layout->addWidget(m_lights, 1);
    layout->addLayout(buttonLayout);
    layout->addSpacing(5);
    layout->addWidget(new QLabel("Détails :"));
    layout->addWidget(m_lightFrame, 2);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setWidget(centralWidget);

    updateButtonsVisibility();

    connect(m_lights, SIGNAL(currentRowChanged(int)), this, SLOT(onIndexChange(int)));
    connect(m_addButton, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
    connect(m_delButton, SIGNAL(clicked(bool)), this, SLOT(onDelClicked()));
}

void LightsDock::changeRoom(std::weak_ptr<Patern> room)
{
    m_room = room;
    updateLightList();
}

void LightsDock::updateButtonsVisibility()
{
    if(m_room.expired())
    {
        m_addButton->setDisabled(true);
        m_delButton->setDisabled(true);
        return;
    }
    m_addButton->setEnabled(true);
    if(m_lights->currentRow() < 0 || m_lights->currentRow() >= m_lights->count())
        m_delButton->setDisabled(true);
    else m_delButton->setEnabled(true);
}


void LightsDock::updateLightList()
{
    m_lights->clear();
    auto r(m_room.lock());
    if(r)
    {
        for(unsigned int i(0) ; i < r->lightCount() ; i++)
        {
            const auto & l(r->light(i));
            m_lights->addItem(QString::fromStdString(lightName(l.type())) + " " + QString::number(l.frameCount()));
        }
    }

    updateButtonsVisibility();
}


void LightsDock::onIndexChange(int index)
{
    updateButtonsVisibility();

    if(m_lightHolder != nullptr)
    {
        delete m_lightHolder;
        m_lightHolder = nullptr;
    }

    auto r(m_room.lock());
    if(!r)
        return;

    if(index < 0 || index >= m_lights->count())
        return;

    m_lightHolder = new LightHolder(r->light(index));
    auto l(m_lightFrame->layout());
    if(l != nullptr)
        l->addWidget(m_lightHolder);
}

void LightsDock::onAddClicked()
{
    auto r(m_room.lock());
    if(!r)
        return;
    r->addLight(Light(LightType::POINT));
    updateLightList();
    m_lights->setCurrentRow(r->lightCount()-1);
}

void LightsDock::onDelClicked()
{
    auto r(m_room.lock());
    if(!r)
        return;
    int index(m_lights->currentRow());
    if(index < 0 || index >= m_lights->count())
        return;
    r->delLightAt(index);
    updateLightList();
}
