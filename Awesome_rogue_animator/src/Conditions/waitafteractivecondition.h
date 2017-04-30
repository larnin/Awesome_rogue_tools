#ifndef WAITAFTERACTIVECONDITION_H
#define WAITAFTERACTIVECONDITION_H

#include "condition.h"
#include "UI/conditionholder.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <memory>

class WaitAfterActiveCondition : public Condition
{
    Q_OBJECT
public:
    WaitAfterActiveCondition(const shared_unique_ptr<Condition> & condition, float time);
    virtual ~WaitAfterActiveCondition() = default;

    virtual void draw(QWidget * parent) override;
    virtual void reset() override;

public slots:
    void onValueChange();

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    shared_unique_ptr<Condition> m_condition;
    bool m_active;
    float m_totalTime;

    ConditionHolder * m_conditionHolder;
    QDoubleSpinBox * m_timeWidget;
    QLabel * m_conditionLabel;
    QLabel * m_timeLabel;
    QVBoxLayout * m_principalLayout;
};

#endif // WAITAFTERACTIVECONDITION_H
