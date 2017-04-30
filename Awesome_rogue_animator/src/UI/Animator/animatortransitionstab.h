#ifndef ANIMATORTRANSITIONSTAB_H
#define ANIMATORTRANSITIONSTAB_H

#include "UI/conditionholder.h"
#include <QWidget>
#include <QListWidget>

class AnimatorTransitionsTab : public QWidget
{
    Q_OBJECT
public:
    AnimatorTransitionsTab(QWidget * parent = nullptr);
    virtual ~AnimatorTransitionsTab() = default;

signals:

public slots:
    void onAddTransition(int animationIndex);
    void onStateIndexChange(int index);
    void onAnimationListChanged();
    void updateCondition();
    void onRightClickOnTransitionList(QPoint point);

private:
    void updateTransitionList(bool resetIndex);

    QListWidget * m_transitions;
    ConditionHolder * m_condition;
    int m_currentStateIndex;
};

#endif // ANIMATORTRANSITIONSTAB_H
