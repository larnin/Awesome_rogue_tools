#ifndef ANIMATORWINDOW_H
#define ANIMATORWINDOW_H

#include "animatoranimationlist.h"
#include <QWidget>

class AnimatorWindow : public QWidget
{
    Q_OBJECT
public:
    AnimatorWindow(QWidget * parent = nullptr);
    virtual ~AnimatorWindow() = default;

public slots:
    void onAnimationListChanged();
private:
    AnimatorAnimationList * m_animationList;
};

#endif // ANIMATORWINDOW_H
