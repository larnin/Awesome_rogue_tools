#ifndef ANDCONDITION_H
#define ANDCONDITION_H

#include "condition.h"
#include <vector>
#include <memory>

class AndCondition : public Condition
{
public:
    AndCondition();
    virtual ~AndCondition() = default;
    void add(const shared_unique_ptr<Condition> & condition);

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    std::vector<shared_unique_ptr<Condition>> m_conditions;
};

#endif // ANDCONDITION_H
