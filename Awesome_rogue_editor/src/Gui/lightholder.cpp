#include "lightholder.h"
#include "Map/light.h"
#include "lightframeholder.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QGroupBox>

LightHolder::LightHolder(Light & light, QWidget * parent)
    : QWidget(parent)
    , m_light(light)
{
    m_lightType = new QComboBox();
    m_lightType->addItem(QString::fromStdString(lightName(LightType::POINT)));
    m_lightType->addItem(QString::fromStdString(lightName(LightType::DIRECTIONNAL)));
    m_lightType->addItem(QString::fromStdString(lightName(LightType::SPOT)));

    m_frame = new QFrame();
    m_frame->setLayout(new QVBoxLayout());
    QScrollArea* area = new QScrollArea();
    area->setWidgetResizable(true);
    area->setWidget(m_frame);

    m_addButton = new QPushButton("Ajouter frame");
    m_delButton = new QPushButton("Suprimer frame");
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_addButton);
    buttonsLayout->addWidget(m_delButton);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(m_lightType);
    layout->addWidget(area);
    layout->addLayout(buttonsLayout);

    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    connect(m_lightType, SIGNAL(currentIndexChanged(int)), this, SLOT(onLightTypeChange(int)));
    connect(m_addButton, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
    connect(m_delButton, SIGNAL(clicked(bool)), this, SLOT(onDelClicked()));

    initializeFrames();
}

void LightHolder::onLightTypeChange(int index)
{
    for(auto & l : m_widgets)
        delete l;
    m_widgets.clear();
    m_light.setType(LightType(index));

    initializeFrames();

    emit lightChanged();
}

void LightHolder::onAddClicked()
{
    m_light.add(LightFrame());
    m_delButton->setDisabled(m_light.frameCount() <= 1);

    add(m_light.type(), m_light.frameCount()-1);
}

void LightHolder::onDelClicked()
{
    m_light.del(m_light.frameCount()-1);
    m_delButton->setDisabled(m_light.frameCount() <= 1);

    delLast();
}

void LightHolder::initializeFrames()
{
    m_delButton->setDisabled(m_light.frameCount() <= 1);
    for(unsigned int i(0) ; i < m_light.frameCount() ; i++)
        add(m_light.type(), i);
}

void LightHolder::add(LightType type, unsigned int index)
{
    QWidget *widget = nullptr;
    LightFrame & f(m_light[index]);
    switch (type) {
    case LightType::POINT:
        widget = new FramePointLightHolder(f);
    break;
    case LightType::SPOT:
        widget = new FrameSpotLightHolder(f);
    break;
    case LightType::DIRECTIONNAL:
        widget = new FrameDirectionnalLightHolder(f);
    break;
    default:
        return;
    }

    if(widget != nullptr)
    {
        QVBoxLayout *l = new QVBoxLayout();
        l->addWidget(widget);
        l->setContentsMargins(0, 0, 0, 0);
        QGroupBox *f = new QGroupBox("Frame " + QString::number(index+1));
        f->setLayout(l);

        m_frame->layout()->addWidget(f);
        m_widgets.push_back(f);
    }

    emit lightChanged();
}

void LightHolder::delLast()
{
    if(m_widgets.empty())
        return;

    delete m_widgets.back();
    m_widgets.pop_back();

    emit lightChanged();
}
