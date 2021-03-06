#ifndef LISTCONDITION_H
#define LISTCONDITION_H

#include "condition.h"
#include <QVBoxLayout>
#include <QPushButton>

class ListCondition : public Condition
{
    Q_OBJECT
public:
    virtual ~ListCondition() = default;
    void add(const shared_unique_ptr<Condition> & condition);

    virtual void draw(QWidget * parent) override;
    virtual void reset() override;

public slots:
    void onAddClicked();
    void onDelClicked();

protected:
    ListCondition(ConditionType type);
    virtual void saveData(QJsonObject & o) const override;

private:
    std::vector<shared_unique_ptr<Condition>> m_conditions;

    std::vector<QWidget *> m_widgets;
    QPushButton  * m_addButton;
    QPushButton * m_delButton;
    QVBoxLayout * m_principalLayout;
};

class AndCondition : public ListCondition
{
public:
    inline AndCondition() : ListCondition(ConditionType::AND_CONDITION) {}
    virtual ~AndCondition() = default;
};

class OrCondition : public ListCondition
{
public:
    inline OrCondition(): ListCondition(ConditionType::OR_CONDITION) {}
    virtual ~OrCondition() = default;
};


#endif // LISTCONDITION_H
