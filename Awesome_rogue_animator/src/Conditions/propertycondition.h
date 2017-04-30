#ifndef PROPERTYCONDITION_H
#define PROPERTYCONDITION_H

#include "condition.h"
#include <string>

class PropertyCondition : public Condition
{
    Q_OBJECT
public:
    virtual ~PropertyCondition() = default;

    virtual void draw(QWidget * parent) override;
    virtual void reset() override;

protected:
    PropertyCondition(const std::string & property, int value, ConditionType type);
    virtual void saveData(QJsonObject & o) const override;

private:
    std::string m_property;
    int m_value;
};

class PropertyEqualCondition : public PropertyCondition
{
public:
    inline PropertyEqualCondition(const std::string & property, int value)
        : PropertyCondition(property, value, ConditionType::PROPERTY_EQUAL_CONDITION) {}
    virtual ~PropertyEqualCondition() = default;
};

class PropertyNotEqualCondition : public PropertyCondition
{
public:
    inline PropertyNotEqualCondition(const std::string & property, int value)
        : PropertyCondition(property, value, ConditionType::PROPERTY_NOT_EQUAL_CONDITION) {}
    virtual ~PropertyNotEqualCondition() = default;
};

class PropertyInferiorCondition : public PropertyCondition
{
public:
    inline PropertyInferiorCondition(const std::string & property, int value)
        : PropertyCondition(property, value, ConditionType::PROPERTY_INFERIOR_CONDITION) {}
    virtual ~PropertyInferiorCondition() = default;
};

class PropertyInferiorOrEqualCondition : public PropertyCondition
{
public:
    inline PropertyInferiorOrEqualCondition(const std::string & property, int value)
        : PropertyCondition(property, value, ConditionType::PROPERTY_INFERIOR_OR_EQUAL_CONDITION) {}
    virtual ~PropertyInferiorOrEqualCondition() = default;
};

class PropertySuperiorCondition : public PropertyCondition
{
public:
    inline PropertySuperiorCondition(const std::string & property, int value)
        : PropertyCondition(property, value, ConditionType::PROPERTY_SUPERIOR_CONDITION) {}
    virtual ~PropertySuperiorCondition() = default;
};

class PropertySuperiorOrEqualCondition : public PropertyCondition
{
public:
    inline PropertySuperiorOrEqualCondition(const std::string & property, int value)
        : PropertyCondition(property, value, ConditionType::PROPERTY_SUPERIOR_OR_EQUAL_CONDITION) {}
    virtual ~PropertySuperiorOrEqualCondition() = default;
};

#endif // PROPERTYCONDITION_H
