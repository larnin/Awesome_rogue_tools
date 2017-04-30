#include "notcondition.h"

NotCondition::NotCondition(const shared_unique_ptr<Condition> & condition)
    : Condition(ConditionType::NOT_CONDITION)
    , m_condition(condition)
{

}

void NotCondition::saveData(QJsonObject & o) const
{
    if(m_condition)
        o.insert("condition", (*m_condition)->save());
}

void NotCondition::draw(QWidget * parent)
{
    m_conditionHolder = new ConditionHolder(m_condition);
    m_label = new QLabel("Condition");

    m_principalLayout = new QVBoxLayout();
    m_principalLayout->addWidget(m_label);
    m_principalLayout->addWidget(m_conditionHolder, 1);

    parent->setLayout(m_principalLayout);
}

void NotCondition::reset()
{
    if(m_condition && *m_condition)
        (*m_condition)->reset();

    m_conditionHolder->hide();
    delete m_conditionHolder;
    m_label->hide();
    delete m_label;

    delete m_principalLayout;
}
