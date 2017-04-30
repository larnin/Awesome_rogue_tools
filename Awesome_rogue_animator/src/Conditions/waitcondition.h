#ifndef WAITCONDITION_H
#define WAITCONDITION_H

#include "condition.h"

class WaitCondition : public Condition
{
    Q_OBJECT
public:
    WaitCondition(float time);
    virtual ~WaitCondition() = default;

    virtual void draw(QWidget * parent) override;
    virtual void reset() override;

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    float m_totalTime;
    float m_currentTime;
};

#endif // WAITCONDITION_H
