#include "listcondition.h"
#include <QJsonArray>

ListCondition::ListCondition(ConditionType type)
    : Condition(type)
{

}

void ListCondition::add(const shared_unique_ptr<Condition> & condition)
{
    m_conditions.emplace_back(condition);
}

void ListCondition::saveData(QJsonObject & o) const
{
    o.insert("conditions", Condition::saveConditionList(m_conditions));
}

void ListCondition::draw(QWidget * parent)
{

}

void ListCondition::reset()
{

}
