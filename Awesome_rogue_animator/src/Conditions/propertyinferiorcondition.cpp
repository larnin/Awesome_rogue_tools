#include "propertyinferiorcondition.h"

PropertyInferiorCondition::PropertyInferiorCondition(const std::string & property, int value)
    : Condition(ConditionType::PROPERTY_INFERIOR_CONDITION)
    , m_property(property)
    , m_value(value)
{

}

void PropertyInferiorCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}
