#include "notcondition.h"

NotCondition::NotCondition(std::unique_ptr<Condition> & condition)
    : Condition(ConditionType::NOT_CONDITION)
    , m_condition(std::move(condition))
{

}

NotCondition::NotCondition(std::unique_ptr<Condition> && condition)
    : Condition(ConditionType::NOT_CONDITION)
    , m_condition(std::move(condition))
{

}

void NotCondition::saveData(QJsonObject & o) const
{
    if(m_condition)
        o.insert("condition", m_condition->save());
}
