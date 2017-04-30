#include "animatortransitionstab.h"
#include "datas.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

AnimatorTransitionsTab::AnimatorTransitionsTab(QWidget * parent)
    : QWidget(parent)
    , m_currentStateIndex(-1)
{
    m_transitions = new QListWidget();
    m_condition = new ConditionHolder(std::make_shared<std::unique_ptr<Condition>>());

    QScrollArea * area = new QScrollArea();
    area->setWidget(m_condition);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Transitions"));
    layout->addWidget(m_transitions);
    layout->addSpacing(5);
    layout->addWidget(new QLabel("Condition"));
    layout->addWidget(area, 1);
    layout->setSpacing(0);
    layout->setMargin(0);

    setLayout(layout);

    connect(m_transitions, SIGNAL(currentRowChanged(int)), this, SLOT(updateCondition()));

    updateCondition();
}

void AnimatorTransitionsTab::updateTransitionList(bool resetIndex)
{
    m_transitions->blockSignals(true);

    int index(m_transitions->currentRow());

    m_transitions->clear();
    if(m_currentStateIndex >= 0 && m_currentStateIndex < int(Datas::instance().size()))
    {
        for(unsigned int i(0) ; i < Datas::instance()[m_currentStateIndex].transitions.size() ; i++)
        {
            const Transition & t(Datas::instance()[m_currentStateIndex].transitions[i]);
            m_transitions->addItem(QString::number(i) + " : " + QString::number(t.targetAnimationID) + " (" + QString::fromStdString(Datas::instance()[t.targetAnimationID].animation.name) + ")");
        }
    }

    if(!resetIndex)
        m_transitions->setCurrentRow(index);
    updateCondition();

    m_transitions->blockSignals(false);
}


void AnimatorTransitionsTab::updateCondition()
{
    bool showCondition(false);
    int index = m_transitions->currentRow();
    if(m_currentStateIndex >= 0 && m_currentStateIndex < int(Datas::instance().size()))
        if(index >= 0 && index < int(Datas::instance()[m_currentStateIndex].transitions.size()))
            showCondition = true;

   if(!showCondition)
   {
       m_condition->setDisabled(true);
       m_condition->setCondition(std::make_shared<std::unique_ptr<Condition>>());
   }
   else
   {
       m_condition->setEnabled(true);
       m_condition->setCondition(Datas::instance()[m_currentStateIndex].transitions[index].condition);
   }
}

void AnimatorTransitionsTab::onAddTransition(int animationIndex)
{
    if(animationIndex == m_currentStateIndex)
        updateTransitionList(false);
}


void AnimatorTransitionsTab::onStateIndexChange(int index)
{
    m_currentStateIndex = index;
    updateTransitionList(true);
}


void AnimatorTransitionsTab::onAnimationListChanged()
{
    updateTransitionList(false);
}
