#ifndef ANIMATIONFINISHEDCONDITION_H
#define ANIMATIONFINISHEDCONDITION_H

#include "condition.h"

class AnimationFinishedCondition : public Condition
{
    Q_OBJECT
public:
    AnimationFinishedCondition();
    virtual ~AnimationFinishedCondition() = default;

    virtual void draw(QWidget * parent) override;
    virtual void reset() override;

protected:
    virtual void saveData(QJsonObject &) const override;
};

#endif // ANIMATIONFINISHEDCONDITION_H
