#ifndef ANIMATORANIMATIONLIST_H
#define ANIMATORANIMATIONLIST_H

#include <QWidget>
#include <QListWidget>
#include <QSpinBox>

class AnimatorAnimationList : public QWidget
{
    Q_OBJECT
public:
    AnimatorAnimationList(QWidget* parent = nullptr);

signals:
    void indexChanged(int);
    void defaultStateChanged(int);

public slots:
    void onAnimationListChanged();
    void onDefaultStateChanged();
    void onItemSelectedChanged(int item);

private:
    void updateAnimationList();
    QListWidget * m_animationList;
    QSpinBox * m_defaultState;
};

#endif // ANIMATORANIMATIONLIST_H
