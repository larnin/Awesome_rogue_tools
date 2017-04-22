#ifndef WAITCONDITION_H
#define WAITCONDITION_H

#include "condition.h"

class WaitCondition : public Condition
{
public:
    WaitCondition(float time);
    virtual ~WaitCondition() = default;

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    float m_totalTime;
    float m_currentTime;
};

#endif // WAITCONDITION_H
