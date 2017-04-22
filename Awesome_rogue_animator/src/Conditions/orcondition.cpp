#include "orcondition.h"
#include <QJsonArray>

OrCondition::OrCondition()
    : Condition(ConditionType::OR_CONDITION)
{

}

void OrCondition::add(std::unique_ptr<Condition> & condition)
{
    m_conditions.emplace_back(std::move(condition));
}

void OrCondition::add(std::unique_ptr<Condition> && condition)
{
    m_conditions.emplace_back(std::move(condition));
}

void OrCondition::saveData(QJsonObject & o) const
{
    o.insert("conditions", Condition::saveConditionList(m_conditions));
}
