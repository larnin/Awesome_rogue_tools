#include "animatoranimationlist.h"
#include "datas.h"
#include <QVBoxLayout>
#include <QLabel>

AnimatorAnimationList::AnimatorAnimationList(QWidget * parent)
    : QWidget(parent)
{
    m_animationList = new QListWidget();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Animations"));
    layout->addWidget(m_animationList);

    setLayout(layout);

    connect(m_animationList, SIGNAL(currentRowChanged(int)), this, SIGNAL(indexChanged(int)));
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
