#include "andcondition.h"
#include <QJsonArray>

AndCondition::AndCondition()
    : Condition(ConditionType::AND_CONDITION)
{

}

void AndCondition::add(const shared_unique_ptr<Condition> & condition)
{
    m_conditions.emplace_back(condition);
}

void AndCondition::saveData(QJsonObject & o) const
{
    o.insert("conditions", Condition::saveConditionList(m_conditions));
}
