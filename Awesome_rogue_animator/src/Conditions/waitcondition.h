#ifndef WAITCONDITION_H
#define WAITCONDITION_H

#include "condition.h"
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QLabel>

class WaitCondition : public Condition
{
    Q_OBJECT
public:
    WaitCondition(float time);
    virtual ~WaitCondition() = default;

    virtual void draw(QWidget * parent) override;
    virtual void reset() override;

public slots:
    void onValueChange();

protected:
    virtual void saveData(QJsonObject & o) const override;

private:
    float m_totalTime;

    QVBoxLayout * m_principalLayout;
    QDoubleSpinBox * m_timeWidget;
    QLabel * m_label;
};

#endif // WAITCONDITION_H
