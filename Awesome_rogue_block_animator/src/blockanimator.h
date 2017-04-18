#ifndef BLOCKANIMATOR_H
#define BLOCKANIMATOR_H

#include <QWidget>
#include <QTreeWidget>
#include "animationviewer.h"

class BlockAnimator : public QWidget
{
    Q_OBJECT

public:
    BlockAnimator(QWidget * parent = nullptr);
    virtual ~BlockAnimator() = default;

public slots:
    void onConfigChanged();
    void currentChanged(QTreeWidgetItem *current);

private:
    struct AnimIndex
    {
        AnimIndex(unsigned _id, int _anim, QTreeWidgetItem * _item)
            : blockID(_id), animID(_anim), item(_item) {}
        unsigned int blockID;
        int animID;
        QTreeWidgetItem *item;
    };

    void rebuildList();
    QTreeWidget * m_animList;
    AnimationViewer * m_viewer;
    std::vector<AnimIndex> m_animListIndexs;
};

#endif // BLOCKANIMATOR_H
