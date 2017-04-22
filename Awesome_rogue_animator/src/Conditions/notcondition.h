#ifndef NOTCONDITION_H
#define NOTCONDITION_H

#include "condition.h"
#include <memory>

class NotCondition : public Condition
{
public:
    NotCondition(std::unique_ptr<Condition> & condition);
    NotCondition(std::unique_ptr<Condition> && condition);
    virtual ~NotCondition() = default;

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    std::unique_ptr<Condition> m_condition;
};

#endif // NOTCONDITION_H
