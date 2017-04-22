#include "propertyequalcondition.h"

PropertyEqualCondition::PropertyEqualCondition(const std::string & property, int value)
    : Condition(ConditionType::PROPERTY_EQUAL_CONDITION)
    , m_property(property)
    , m_value(value)
{

}

void PropertyEqualCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}
