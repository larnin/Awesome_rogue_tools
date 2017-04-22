#include "condition.h"
#include "andcondition.h"
#include "orcondition.h"
#include "notcondition.h"
#include "animationfinishedcondition.h"
#include "waitcondition.h"
#include "waitafteractivecondition.h"
#include "propertyequalcondition.h"
#include "propertynotequalcondition.h"
#include "propertyinferiorcondition.h"
#include "propertyinferiororequalcondition.h"
#include "propertysuperiorcondition.h"
#include "propertysuperiororequalcondition.h"
#include <QJsonArray>
#include <utility>
#include <string>

Condition::Condition(ConditionType type)
    : m_conditionType(type)
{

}

QJsonObject Condition::save() const
{
    QJsonObject object;
    object.insert("type", int(m_conditionType));
    saveData(object);
    return object;
}

std::pair<std::string, int> getProperty(const QJsonObject & j)
{
    return std::make_pair<std::string, int>(j["property"].toString().toStdString(), j["value"].toInt());
}

std::vector<std::unique_ptr<Condition>> getConditionList(const QJsonObject & j)
{
    auto it(j.find("conditions"));
    if(it == j.end() || !it->isArray())
        return std::vector<std::unique_ptr<Condition>>();

    std::vector<std::unique_ptr<Condition>> conditions;

    for(auto jItem : it->toArray())
        conditions.push_back(std::move(Condition::load(jItem.toObject())));
    return conditions;
}

std::unique_ptr<Condition> Condition::load(const QJsonObject & j)
{
    auto itType(j.find("type"));
    if(itType == j.end())
        return std::unique_ptr<Condition>();

    switch (itType->toInt())
    {
    case AND_CONDITION:
    {
        auto condition = std::make_unique<AndCondition>();
        for(auto & c : getConditionList(j))
            condition->add(c);
        return condition;
    }
    case OR_CONDITION:
    {
        auto condition = std::make_unique<OrCondition>();
        for(auto & c : getConditionList(j))
            condition->add(c);
        return condition;
    }
    case NOT_CONDITION:
    {
        auto itCondition(j.find("condition"));
        if(itCondition == j.end())
            return std::unique_ptr<Condition>();
        return std::make_unique<NotCondition>(load(itCondition->toObject()));
    }
    case ANIMATION_FINISHED_CONDITION:
    {
        return std::make_unique<AnimationFinishedCondition>();
    }
    case WAIT_CONDITION:
    {
        return std::make_unique<WaitCondition>(j["time"].toInt());
    }
    case WAIT_AFTER_ACTIVE_CONDITION:
    {
        auto itCondition(j.find("condition"));
        if(itCondition == j.end())
            return std::unique_ptr<Condition>();
        return std::make_unique<WaitAfterActiveCondition>(load(itCondition->toObject()), j["time"].toInt());
    }
    case PROPERTY_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyEqualCondition>(property.first, property.second);
    }
    case PROPERTY_NOT_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyNotEqualCondition>(property.first, property.second);
    }
    case PROPERTY_INFERIOR_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyInferiorCondition>(property.first, property.second);
    }
    case PROPERTY_INFERIOR_OR_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyInferiorOrEqualCondition>(property.first, property.second);
    }
    case PROPERTY_SUPERIOR_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertySuperiorCondition>(property.first, property.second);
    }
    case PROPERTY_SUPERIOR_OR_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertySuperiorOrEqualCondition>(property.first, property.second);
    }
    default:
        return std::unique_ptr<Condition>();
    }
}

void Condition::saveProperty(QJsonObject & obj, const std::string & property, int value)
{
    obj.insert("property", QString::fromStdString(property));
    obj.insert("value", value);
}

QJsonArray Condition::saveConditionList(const std::vector<std::unique_ptr<Condition>> & list)
{
    QJsonArray array;
    for(const auto & value : list)
    {
        if(value)
            array.append(value->save());
    }
    return array;
}
