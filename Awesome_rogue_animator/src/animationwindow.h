#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include "animationlist.h"
#include "animationpreview.h"
#include <QWidget>

class AnimationWindow : public QWidget
{
    Q_OBJECT
public:
    AnimationWindow(QWidget *parent = nullptr);
    virtual ~AnimationWindow() = default;

signals:

public slots:

private:
    AnimationList * m_animationList;
    AnimationPreview * m_animationPreview;

};

#endif // ANIMATIONWINDOW_H
