#include "propertyinferiororequalcondition.h"

PropertyInferiorOrEqualCondition::PropertyInferiorOrEqualCondition(const std::string & property, int value)
    : Condition(ConditionType::PROPERTY_INFERIOR_OR_EQUAL_CONDITION)
    , m_property(property)
    , m_value(value)
{

}

void PropertyInferiorOrEqualCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}
