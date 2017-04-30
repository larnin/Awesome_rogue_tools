#include "animationfinishedcondition.h"

AnimationFinishedCondition::AnimationFinishedCondition()
    : Condition(ConditionType::ANIMATION_FINISHED_CONDITION)
{

}

void AnimationFinishedCondition::saveData(QJsonObject &) const
{

}

void AnimationFinishedCondition::draw(QWidget * parent)
{
    m_label = new QLabel("Pas de paramètre");

    m_principalLayout = new QVBoxLayout();
    m_principalLayout->addWidget(m_label);
    m_principalLayout->addStretch(1);

    parent->setLayout(m_principalLayout);
}

void AnimationFinishedCondition::reset()
{
    m_label->hide();
    delete m_label;

    delete m_principalLayout;
}
