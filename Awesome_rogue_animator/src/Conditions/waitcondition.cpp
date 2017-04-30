#include "waitcondition.h"
#include <QHBoxLayout>

WaitCondition::WaitCondition(float time)
    : Condition(ConditionType::WAIT_CONDITION)
    , m_totalTime(time)
{

}

void WaitCondition::saveData(QJsonObject & o) const
{
    o.insert("time", m_totalTime);
}

void WaitCondition::draw(QWidget * parent)
{
    m_timeWidget = new QDoubleSpinBox();
    m_timeWidget->setRange(0, 10000);
    m_timeWidget->setSingleStep(0.05);
    m_timeWidget->setValue(m_totalTime);

    m_label = new QLabel("Temps (s) : ");

    QHBoxLayout * timeLayout = new QHBoxLayout();
    timeLayout->addWidget(m_label);
    timeLayout->addWidget(m_timeWidget, 1);

    m_principalLayout = new QVBoxLayout();
    m_principalLayout->addLayout(timeLayout);
    m_principalLayout->addStretch(1);

    parent->setLayout(m_principalLayout);

    connect(m_timeWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChange()));
}

void WaitCondition::reset()
{
    disconnect(m_timeWidget, SIGNAL(valueChanged(double)), 0, 0);

    m_timeWidget->hide();
    delete m_timeWidget;
    m_label->hide();
    delete m_label;

    delete m_principalLayout;
}

void WaitCondition::onValueChange()
{
    m_totalTime = m_timeWidget->value();
}
