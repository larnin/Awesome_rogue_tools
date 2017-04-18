#include "blockanimator.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QHeaderView>
#include <tilesetdata.h>
#include <algorithm>

BlockAnimator::BlockAnimator(QWidget * parent)
    : QWidget(parent)
{
    setWindowFlags ( Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint);
    setWindowTitle("Animation viewer");

    m_animList = new QTreeWidget();
    m_animList->header()->hide();
    m_viewer = new AnimationViewer();
    m_viewer->setFixedSize(74, 74);
    QFrame * frame = new QFrame();
    QVBoxLayout * viewerLayout = new QVBoxLayout();
    viewerLayout->addWidget(m_viewer);
    frame->setLayout(viewerLayout);
    frame->setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Animations :"));
    layout->addWidget(m_animList);
    layout->addSpacing(5);
    layout->addWidget(new QLabel("Appercu :"));
    layout->addWidget(frame, 0, Qt::AlignHCenter);

    setLayout(layout);

    connect(m_animList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(currentChanged(QTreeWidgetItem*)));
}

void BlockAnimator::onConfigChanged()
{
    rebuildList();
    currentChanged(m_animList->currentItem());
}

void BlockAnimator::rebuildList()
{
    QTreeWidgetItem *current(m_animList->currentItem());
    auto it(std::find_if(m_animListIndexs.begin(), m_animListIndexs.end(), [current](const auto & i){return i.item == current;}));
    bool found(it!=m_animListIndexs.end());
    unsigned int ID(found ? it->blockID : 0);
    int anim(found ? it->animID : 0);

    m_animList->clear();
    m_animListIndexs.clear();
    for(const Animation & a : TilesetData::instance.animations)
    {
        QList<QTreeWidgetItem*> childs;
        for(unsigned int i(1) ; i <= a.anims.size() ; i++)
        {
            if(a.anims[i-1].frames.empty())
                continue;

            QTreeWidgetItem* item(new QTreeWidgetItem({"Animation " + QString::number(i)}));
            childs.push_back(item);
            m_animListIndexs.push_back(AnimIndex(a.id, i, item));
        }
        if(!childs.isEmpty())
        {
            QTreeWidgetItem* item = new QTreeWidgetItem({"Block " + QString::number(a.id)});
            item->addChildren(childs);
            m_animList->addTopLevelItem(item);
            m_animListIndexs.push_back(AnimIndex(a.id, -1, item));
        }
    }

    if(found)
    {
        auto it(std::find_if(m_animListIndexs.begin(), m_animListIndexs.end(), [ID, anim](const auto & i){return i.blockID == ID && i.animID == anim;}));
        if(it != m_animListIndexs.end())
            m_animList->setCurrentItem(it->item);
    }
}

void BlockAnimator::currentChanged(QTreeWidgetItem *current)
{
    auto it(std::find_if(m_animListIndexs.begin(), m_animListIndexs.end(), [current](const auto & i){return i.item == current;}));
    if(it == m_animListIndexs.end())
        m_viewer->setAnimationIndex(-1, -1);

    m_viewer->setAnimationIndex(it->blockID, it->animID);
}
