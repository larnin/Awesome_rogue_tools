#include "animatoranimationlist.h"
#include "datas.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

AnimatorAnimationList::AnimatorAnimationList(QWidget * parent)
    : QWidget(parent)
{
    m_animationList = new QListWidget();
    m_defaultState = new QSpinBox();
    m_defaultState->setMaximum(9999);

    QHBoxLayout *defaultStateLayout = new QHBoxLayout();
    defaultStateLayout->addWidget(new QLabel("Etat par defaut : "));
    defaultStateLayout->addWidget(m_defaultState, 1);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addLayout(defaultStateLayout);
    layout->addWidget(new QLabel("Animations"));
    layout->addWidget(m_animationList);

    setLayout(layout);

    connect(m_animationList, SIGNAL(currentRowChanged(int)), this, SIGNAL(indexChanged(int)));
    connect(m_defaultState, SIGNAL(valueChanged(int)), this, SLOT(onDefaultStateChanged()));
}

void AnimatorAnimationList::onAnimationListChanged()
{
    updateAnimationList();
}

void AnimatorAnimationList::updateAnimationList()
{
    m_animationList->blockSignals(true);

    int index(m_animationList->currentRow());

    m_animationList->clear();

    for(const State & s : Datas::instance())
        m_animationList->addItem(QString::number(m_animationList->count()) + " : " + QString::fromStdString(s.animation.name));

    if(m_animationList->count() > index)
        m_animationList->setCurrentRow(index);
    else emit indexChanged(m_animationList->currentRow());

    m_animationList->blockSignals(false);
}

void AnimatorAnimationList::onDefaultStateChanged()
{
    m_defaultState->blockSignals(true);

    if(Datas::instance().empty())
        m_defaultState->setValue(0);
    else
    {
        if(m_defaultState->value() >= int(Datas::instance().size()))
            m_defaultState->setValue(Datas::instance().size()-1);
        Datas::instance().startIndex = m_defaultState->value();
        emit defaultStateChanged(Datas::instance().startIndex);
    }

    m_defaultState->blockSignals(false);
}

void AnimatorAnimationList::onDefaultStateChangedOut(int index)
{
    m_defaultState->blockSignals(true);

    m_defaultState->setValue(index);
    onDefaultStateChanged();

    m_defaultState->blockSignals(false);
}

void AnimatorAnimationList::onItemSelectedChanged(int item)
{
    m_animationList->blockSignals(true);
    m_animationList->setCurrentRow(item);
    m_animationList->blockSignals(false);
}
