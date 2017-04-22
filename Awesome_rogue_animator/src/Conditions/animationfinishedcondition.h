#ifndef ANIMATIONFINISHEDCONDITION_H
#define ANIMATIONFINISHEDCONDITION_H

#include "condition.h"

class AnimationFinishedCondition : public Condition
{
public:
    AnimationFinishedCondition();
    virtual ~AnimationFinishedCondition() = default;

protected:
    virtual void saveData(QJsonObject &) const override;
};

#endif // ANIMATIONFINISHEDCONDITION_H
