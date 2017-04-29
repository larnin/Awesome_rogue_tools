#include "conditionholder.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

ConditionHolder::ConditionHolder(shared_unique_ptr<Condition> c, QWidget * parent)
    : QWidget(parent)
    , m_condition(c)
{
    m_conditionType = new QComboBox();
    m_conditionType->addItem("Et");
    m_conditionType->addItem("Ou");
    m_conditionType->addItem("Non");
    m_conditionType->addItem("Animation terminée");
    m_conditionType->addItem("Attendre");
    m_conditionType->addItem("Attendre après condition");
    m_conditionType->addItem("Propriétée égale");
    m_conditionType->addItem("Propriétée inégale");
    m_conditionType->addItem("Propriétée inférieure");
    m_conditionType->addItem("Propriétée inférieure ou égale");
    m_conditionType->addItem("Propriétée supérieure");
    m_conditionType->addItem("Propriétée supérieure ou égale");

    m_conditionBorder = new QFrame();
    m_conditionBorder->setFrameShape(QFrame::StyledPanel);

    QHBoxLayout * typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Type :"));
    typeLayout->addWidget(m_conditionType, 1);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addLayout(typeLayout);
    layout->addWidget(m_conditionBorder);

    setLayout(layout);

    onConditionChange();
}

void ConditionHolder::setCondition(const shared_unique_ptr<Condition> & c)
{
    m_condition = c;
    onConditionChange();
}

void ConditionHolder::onConditionChange()
{
    m_conditionType->blockSignals(true);

    if(m_condition && *m_condition)
        m_conditionType->setCurrentIndex(conditionTypeToIndex((*m_condition)->type()));

    m_conditionType->blockSignals(true);
}

int ConditionHolder::conditionTypeToIndex(ConditionType type)
{
    switch(type)
    {
    case AND_CONDITION: return 0;
    case OR_CONDITION: return 1;
    case NOT_CONDITION: return 2;
    case ANIMATION_FINISHED_CONDITION: return 3;
    case WAIT_CONDITION: return 4;
    case WAIT_AFTER_ACTIVE_CONDITION: return 5;
    case PROPERTY_EQUAL_CONDITION: return 6;
    case PROPERTY_NOT_EQUAL_CONDITION: return 7;
    case PROPERTY_INFERIOR_CONDITION: return 8;
    case PROPERTY_INFERIOR_OR_EQUAL_CONDITION: return 9;
    case PROPERTY_SUPERIOR_CONDITION : return 10;
    case PROPERTY_SUPERIOR_OR_EQUAL_CONDITION: return 11;
    default : return 6; //PROPERTY_EQUAL_CONDITION;
    }
}

ConditionType ConditionHolder::indexToConditionType(int index)
{
    switch(index)
    {
    case 0: return AND_CONDITION;
    case 1: return OR_CONDITION;
    case 2: return NOT_CONDITION;
    case 3: return ANIMATION_FINISHED_CONDITION;
    case 4: return WAIT_CONDITION;
    case 5: return WAIT_AFTER_ACTIVE_CONDITION;
    case 6: return PROPERTY_EQUAL_CONDITION;
    case 7: return PROPERTY_NOT_EQUAL_CONDITION;
    case 8: return PROPERTY_INFERIOR_CONDITION;
    case 9: return PROPERTY_INFERIOR_OR_EQUAL_CONDITION;
    case 10: return PROPERTY_SUPERIOR_CONDITION;
    case 11: return PROPERTY_SUPERIOR_OR_EQUAL_CONDITION;
    default: return PROPERTY_EQUAL_CONDITION;
    }
}
