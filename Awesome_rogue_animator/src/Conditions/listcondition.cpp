#include "listcondition.h"
#include <QJsonArray>
#include <UI/conditionholder.h>

ListCondition::ListCondition(ConditionType type)
    : Condition(type)
{

}

void ListCondition::add(const shared_unique_ptr<Condition> & condition)
{
    m_conditions.emplace_back(condition);
}

void ListCondition::saveData(QJsonObject & o) const
{
    o.insert("conditions", Condition::saveConditionList(m_conditions));
}

QWidget * createHorisontalLine()
{
    QWidget * line = new QWidget();
    line->setFixedHeight(1);
    line->setStyleSheet(QString("background-color: #c0c0c0;"));
    return line;
}

void ListCondition::draw(QWidget * parent)
{
    for(unsigned int i = 0 ; i < m_conditions.size() ; i++)
    {
        m_widgets.push_back(new ConditionHolder(m_conditions[i]));
        if(i < m_conditions.size() - 1)
            m_widgets.push_back(createHorisontalLine());
    }

    m_addButton = new QPushButton("Add");
    m_delButton = new QPushButton("Del");

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_addButton);
    buttonsLayout->addWidget(m_delButton);

    m_principalLayout = new QVBoxLayout();
    for(auto & w : m_widgets)
        m_principalLayout->addWidget(w);
    m_principalLayout->addLayout(buttonsLayout);
    m_principalLayout->addStretch(1);

    parent->setLayout(m_principalLayout);

    connect(m_addButton, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
    connect(m_delButton, SIGNAL(clicked(bool)), this, SLOT(onDelClicked()));
}

void ListCondition::reset()
{
    for(auto & c : m_conditions)
        if(c && *c)
            (*c)->reset();

    disconnect(m_addButton, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_delButton, SIGNAL(clicked(bool)), 0, 0);

    for(auto & w : m_widgets)
    {
        w->hide();
        delete w;
    }
    m_widgets.clear();

    m_addButton->hide();
    delete m_addButton;
    m_delButton->hide();
    delete m_delButton;

    delete m_principalLayout;
}

void ListCondition::onAddClicked()
{
    m_conditions.push_back(std::make_shared<std::unique_ptr<Condition>>(Condition::create(ConditionType::PROPERTY_EQUAL_CONDITION)));

    if(!m_widgets.empty())
        m_widgets.push_back(createHorisontalLine());
    m_widgets.push_back(new ConditionHolder(m_conditions.back()));

    if(m_widgets.size() > 1)
        m_principalLayout->insertWidget(m_principalLayout->count()-2, m_widgets[m_widgets.size()-2]);
    m_principalLayout->insertWidget(m_principalLayout->count()-2, m_widgets.back());
}

void ListCondition::onDelClicked()
{
    if(m_conditions.empty())
        return;
    m_conditions.pop_back();

    m_widgets.back()->hide();
    delete m_widgets.back();
    m_widgets.pop_back();

    if(!m_widgets.empty())
    {
        m_widgets.back()->hide();
        delete m_widgets.back();
        m_widgets.pop_back();
    }

    delete m_principalLayout;
}
