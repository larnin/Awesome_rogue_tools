#ifndef CONDITIONHOLDER_H
#define CONDITIONHOLDER_H

#include "Conditions/condition.h"
#include <QWidget>
#include <QComboBox>
#include <memory>
#include <QFrame>

class ConditionHolder : public QWidget
{
    Q_OBJECT
public:
    ConditionHolder(shared_unique_ptr<Condition> c, QWidget * parent = nullptr);
    virtual ~ConditionHolder() = default;

    void setCondition(const shared_unique_ptr<Condition> & c);

    static int conditionTypeToIndex(ConditionType type);
    static ConditionType indexToConditionType(int index);

public slots:
    void onConditionTypeChange();

private:
    void onConditionChange();

    QComboBox * m_conditionType;
    shared_unique_ptr<Condition> m_condition;
    QFrame * m_conditionBorder;
};

#endif // CONDITIONHOLDER_H
