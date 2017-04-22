#include "propertysuperiororequalcondition.h"

PropertySuperiorOrEqualCondition::PropertySuperiorOrEqualCondition(const std::string & property, int value)
    : Condition(ConditionType::PROPERTY_SUPERIOR_OR_EQUAL_CONDITION)
    , m_property(property)
    , m_value(value)
{

}

void PropertySuperiorOrEqualCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}
