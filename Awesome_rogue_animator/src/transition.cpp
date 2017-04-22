#include "transition.h"

Transition::Transition(const QJsonObject & j)
{
    auto itCondition(j.find("condition"));
    if(itCondition != j.end() && itCondition->isObject())
        condition = std::move(Condition::load(itCondition->toObject()));
    auto itTargetAnimation(j.find("target"));
    if(itTargetAnimation != j.end())
        targetAnimationID = itTargetAnimation->toInt();
}

Transition::Transition(unsigned int _targetAnimationID, std::unique_ptr<Condition> && _condition)
    : targetAnimationID(_targetAnimationID)
    , condition(std::move(_condition))
{

}

QJsonObject Transition::toJson() const
{
    QJsonObject object;
    object.insert("target", (int)targetAnimationID);
    if(condition)
        object.insert("condition", condition->save());
    return object;
}
