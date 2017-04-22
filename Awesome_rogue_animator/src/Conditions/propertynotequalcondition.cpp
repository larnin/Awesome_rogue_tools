#include "propertynotequalcondition.h"

PropertyNotEqualCondition::PropertyNotEqualCondition(const std::string & property, int value)
    : Condition(ConditionType::PROPERTY_NOT_EQUAL_CONDITION)
    , m_property(property)
    , m_value(value)
{

}

void PropertyNotEqualCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}
