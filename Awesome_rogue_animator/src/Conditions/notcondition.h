#ifndef NOTCONDITION_H
#define NOTCONDITION_H

#include "condition.h"
#include "UI/conditionholder.h"
#include <QLabel>
#include <QVBoxLayout>
#include <memory>

class NotCondition : public Condition
{
    Q_OBJECT
public:
    NotCondition(const shared_unique_ptr<Condition> & condition);
    virtual ~NotCondition() = default;

    virtual void draw(QWidget * parent) override;
    virtual void reset() override;

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    shared_unique_ptr<Condition> m_condition;

    ConditionHolder * m_conditionHolder;
    QLabel * m_label;
    QVBoxLayout * m_principalLayout;
};

#endif // NOTCONDITION_H
