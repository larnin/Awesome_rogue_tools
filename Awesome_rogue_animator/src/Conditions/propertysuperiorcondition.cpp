#include "propertysuperiorcondition.h"

PropertySuperiorCondition::PropertySuperiorCondition(const std::string & property, int value)
    : Condition(ConditionType::PROPERTY_SUPERIOR_CONDITION)
    , m_property(property)
    , m_value(value)
{

}

void PropertySuperiorCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}
