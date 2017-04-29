#ifndef ORCONDITION_H
#define ORCONDITION_H

#include "condition.h"
#include <vector>
#include <memory>

class OrCondition : public Condition
{
public:
    OrCondition();
    virtual ~OrCondition() = default;
    void add(const shared_unique_ptr<Condition> & condition);

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    std::vector<shared_unique_ptr<Condition>> m_conditions;
};

#endif // ORCONDITION_H
