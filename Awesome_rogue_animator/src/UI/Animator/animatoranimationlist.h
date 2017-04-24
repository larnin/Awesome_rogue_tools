#ifndef ANIMATORANIMATIONLIST_H
#define ANIMATORANIMATIONLIST_H

#include <QWidget>
#include <QListWidget>

class AnimatorAnimationList : public QWidget
{
    Q_OBJECT
public:
    AnimatorAnimationList(QWidget* parent = nullptr);

signals:
    void indexChanged(int);

public slots:
    void onAnimationListChanged();

private:
    void updateAnimationList();
    QListWidget * m_animationList;
};

#endif // ANIMATORANIMATIONLIST_H
