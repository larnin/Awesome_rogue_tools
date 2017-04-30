#include "propertycondition.h"

PropertyCondition::PropertyCondition(const std::string & property, int value, ConditionType type)
    : Condition(type)
    , m_property(property)
    , m_value(value)
{

}

void PropertyCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}

void PropertyCondition::draw(QWidget * parent)
{

}

void PropertyCondition::reset()
{

}
