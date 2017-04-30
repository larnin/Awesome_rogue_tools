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

}

void NotCondition::reset()
{

}
