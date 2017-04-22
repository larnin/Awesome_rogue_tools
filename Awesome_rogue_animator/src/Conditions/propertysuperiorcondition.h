#ifndef PROPERTYSUPERIORCONDITION_H
#define PROPERTYSUPERIORCONDITION_H

#include "condition.h"
#include <string>

class PropertySuperiorCondition : public Condition
{
public:
    PropertySuperiorCondition(const std::string & property, int value);
    virtual ~PropertySuperiorCondition() = default;

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYSUPERIORCONDITION_H
