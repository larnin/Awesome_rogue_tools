#ifndef TRANSITION_H
#define TRANSITION_H

#include "Conditions/condition.h"
#include <SFML/System/Time.hpp>
#include <QJsonObject>
#include <memory>

class Transition
{
public:
    Transition(const QJsonObject & j);
    Transition(unsigned int _targetAnimationID, const shared_unique_ptr<Condition> & _condition);
    Transition(Transition &&) = default;
    Transition & operator =(Transition &&) = default;

    QJsonObject toJson() const;

    unsigned int targetAnimationID;
    shared_unique_ptr<Condition> condition;
};

#endif // TRANSITION_H
