#include "propertycondition.h"
#include "QHBoxLayout"

PropertyCondition::PropertyCondition(const std::string & property, int value, ConditionType type)
    : Condition(type)
    , m_property(property)
    , m_value(value)
{

}

void PropertyCondition::saveData(QJsonObject & o) const
{
    Condition::saveProperty(o, m_property, m_value);
}

void PropertyCondition::draw(QWidget * parent)
{
    m_propertyWidget = new QLineEdit(QString::fromStdString(m_property));
    m_valueWidget = new QSpinBox();
    m_valueWidget->setRange(-10000, 10000);
    m_valueWidget->setValue(m_value);
    m_labelProperty = new QLabel("Propriété : ");
    m_labelValue = new QLabel("Valeur : ");

    QHBoxLayout * propertyLayout = new QHBoxLayout();
    propertyLayout->addWidget(m_labelProperty);
    propertyLayout->addWidget(m_propertyWidget, 1);

    QHBoxLayout * valueLayout = new QHBoxLayout();
    valueLayout->addWidget(m_labelValue);
    valueLayout->addWidget(m_valueWidget, 1);

    m_principalLayout = new QVBoxLayout();
    m_principalLayout->addLayout(propertyLayout);
    m_principalLayout->addLayout(valueLayout);
    m_principalLayout->addStretch(1);

    parent->setLayout(m_principalLayout);

    connect(m_propertyWidget, SIGNAL(textChanged(QString)), this, SLOT(onValueChange()));
    connect(m_valueWidget, SIGNAL(valueChanged(int)), this, SLOT(onValueChange()));
}

void PropertyCondition::reset()
{
    disconnect(m_propertyWidget, SIGNAL(textChanged(QString)), 0, 0);
    disconnect(m_valueWidget, SIGNAL(valueChanged(int)), 0, 0);

    m_propertyWidget->hide();
    delete m_propertyWidget;
    m_valueWidget->hide();
    delete m_valueWidget;
    m_labelProperty->hide();
    delete m_labelProperty;
    m_labelValue->hide();
    delete m_labelValue;

    delete m_principalLayout;
}

void PropertyCondition::onValueChange()
{
    m_property = m_propertyWidget->text().toStdString();
    m_value = m_valueWidget->value();
}
