#ifndef PROPERTYINFERIORCONDITION_H
#define PROPERTYINFERIORCONDITION_H

#include "condition.h"
#include <string>

class PropertyInferiorCondition : public Condition
{
public:
    PropertyInferiorCondition(const std::string & property, int value);
    virtual ~PropertyInferiorCondition() = default;

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYINFERIORCONDITION_H
