#include "waitafteractivecondition.h"

WaitAfterActiveCondition::WaitAfterActiveCondition(const shared_unique_ptr<Condition> & condition, float time)
    : Condition(ConditionType::WAIT_AFTER_ACTIVE_CONDITION)
    , m_condition(condition)
    , m_active(false)
    , m_totalTime(time)
    , m_currentTime(0)
{

}

void WaitAfterActiveCondition::saveData(QJsonObject & o) const
{
    if(m_condition)
        o.insert("condition", (*m_condition)->save());
    o.insert("time", m_totalTime);
}
