#include "lightsdock.h"
#include "Utilities/configs.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QColorDialog>

namespace
{
QIcon generateColor(const sf::Color & color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(QColor(color.r, color.g, color.b, color.a));
    return QIcon(pixmap);
}
}

LightsDock::LightsDock(QWidget *parent)
    : UnclosableDock("Lumieres", parent)
    , m_lightHolder(nullptr)
{
    m_ambiantWidget = new QPushButton(generateColor(Configs::tiles.ambiantColor), "");
    m_ambiantWidget->setFixedWidth(25);

    m_lights = new QListWidget();
    m_lightFrame = new QFrame();
    m_lightFrame->setLayout(new QVBoxLayout());
    m_lightFrame->setFrameShape(QFrame::Box);

    m_addButton = new QPushButton("Ajouter");
    m_delButton = new QPushButton("Suprimer");
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_delButton);

    QHBoxLayout* ambiantLayout = new QHBoxLayout();
    ambiantLayout->addWidget(new QLabel("Ambiant :"));
    ambiantLayout->addWidget(m_ambiantWidget);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addLayout(ambiantLayout);
    layout->addSpacing(5);
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
    connect(m_ambiantWidget, SIGNAL(clicked(bool)), this, SLOT(onAmbiantClicked()));
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
        disconnect(m_lightHolder, SIGNAL(lightChanged()), 0, 0);
        delete m_lightHolder;
        m_lightHolder = nullptr;
    }

    auto r(m_room.lock());
    if(!r)
        return;

    if(index < 0 || index >= m_lights->count())
        return;

    m_lightHolder = new LightHolder(r->light(index));
    connect(m_lightHolder, SIGNAL(lightChanged()), this, SLOT(onLightChange()));
    auto l(m_lightFrame->layout());
    if(l != nullptr)
        l->addWidget(m_lightHolder);
}

void LightsDock::onAddClicked()
{
    auto r(m_room.lock());
    if(!r)
        return;
    Light l(LightType::POINT);
    l.add(LightFrame());
    r->addLight(l);
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

void LightsDock::onLightChange()
{
    int row(m_lights->currentRow());
    m_lights->blockSignals(true);
    updateLightList();
    m_lights->setCurrentRow(row);
    m_lights->blockSignals(false);
}

void LightsDock::onAmbiantClicked()
{
    auto& color(Configs::tiles.ambiantColor);
    QColor c (QColorDialog::getColor(QColor(color.r, color.g, color.b), this, "Couleur ambiante"));
    if(!c.isValid())
        return;

    color = sf::Color(c.red(), c.green(), c.blue());
    m_ambiantWidget->setIcon(generateColor(color));
}

