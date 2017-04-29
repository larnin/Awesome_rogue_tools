#ifndef WAITAFTERACTIVECONDITION_H
#define WAITAFTERACTIVECONDITION_H

#include "condition.h"
#include <memory>

class WaitAfterActiveCondition : public Condition
{
public:
    WaitAfterActiveCondition(const shared_unique_ptr<Condition> & condition, float time);
    virtual ~WaitAfterActiveCondition() = default;

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    shared_unique_ptr<Condition> m_condition;
    bool m_active;
    float m_totalTime;
    float m_currentTime;

};

#endif // WAITAFTERACTIVECONDITION_H
