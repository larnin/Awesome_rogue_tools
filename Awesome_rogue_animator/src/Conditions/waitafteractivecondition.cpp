#include "waitafteractivecondition.h"
#include <QHBoxLayout>

WaitAfterActiveCondition::WaitAfterActiveCondition(const shared_unique_ptr<Condition> & condition, float time)
    : Condition(ConditionType::WAIT_AFTER_ACTIVE_CONDITION)
    , m_condition(condition)
    , m_active(false)
    , m_totalTime(time)
{

}

void WaitAfterActiveCondition::saveData(QJsonObject & o) const
{
    if(m_condition)
        o.insert("condition", (*m_condition)->save());
    o.insert("time", m_totalTime);
}

void WaitAfterActiveCondition::draw(QWidget * parent)
{
    m_conditionHolder = new ConditionHolder(m_condition);

    m_timeWidget = new QDoubleSpinBox();
    m_timeWidget->setRange(0, 10000);
    m_timeWidget->setSingleStep(0.05);
    m_timeWidget->setValue(m_totalTime);

    m_timeLabel = new QLabel("Temps (s) : ");
    m_conditionLabel = new QLabel("Condition");

    QHBoxLayout * timeLayout = new QHBoxLayout();
    timeLayout->addWidget(m_timeLabel);
    timeLayout->addWidget(m_timeWidget, 1);

    m_principalLayout = new QVBoxLayout();
    m_principalLayout->addLayout(timeLayout);
    m_principalLayout->addSpacing(5);
    m_principalLayout->addWidget(m_conditionLabel);
    m_principalLayout->addWidget(m_conditionHolder, 1);

    parent->setLayout(m_principalLayout);

    connect(m_timeWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChange()));
}

void WaitAfterActiveCondition::reset()
{
    if(m_condition && *m_condition)
        (*m_condition)->reset();

    disconnect(m_timeWidget, SIGNAL(valueChanged(double)), 0, 0);

    m_conditionHolder->hide();
    delete m_conditionHolder;
    m_timeWidget->hide();
    delete m_timeWidget;
    m_conditionLabel->hide();
    delete m_conditionLabel;
    m_timeLabel->hide();
    delete m_timeLabel;

    delete m_principalLayout;
}

void WaitAfterActiveCondition::onValueChange()
{
    m_totalTime = m_timeWidget->value();
}
