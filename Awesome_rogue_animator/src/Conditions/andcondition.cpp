#include "andcondition.h"
#include <QJsonArray>

AndCondition::AndCondition()
    : Condition(ConditionType::AND_CONDITION)
{

}

void AndCondition::add(std::unique_ptr<Condition> & condition)
{
    m_conditions.emplace_back(std::move(condition));
}

void AndCondition::add(std::unique_ptr<Condition> && condition)
{
    m_conditions.emplace_back(std::move(condition));
}

void AndCondition::saveData(QJsonObject & o) const
{
    o.insert("conditions", Condition::saveConditionList(m_conditions));
}
