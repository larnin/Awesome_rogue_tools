#ifndef ANIMATIONFINISHEDCONDITION_H
#define ANIMATIONFINISHEDCONDITION_H

#include "condition.h"
#include <QVBoxLayout>
#include <QLabel>

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

private:
    QLabel * m_label;
    QVBoxLayout * m_principalLayout;
};

#endif // ANIMATIONFINISHEDCONDITION_H
