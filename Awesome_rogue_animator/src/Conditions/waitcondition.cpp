#include "waitcondition.h"

WaitCondition::WaitCondition(float time)
    : Condition(ConditionType::WAIT_CONDITION)
    , m_totalTime(time)
    , m_currentTime(0)
{

}

void WaitCondition::saveData(QJsonObject & o) const
{
    o.insert("time", m_totalTime);
}
