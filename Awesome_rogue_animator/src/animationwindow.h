#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include "animationlist.h"
#include "animationpreview.h"
#include "animationtextureview.h"
#include <QWidget>

class AnimationWindow : public QWidget
{
    Q_OBJECT
public:
    AnimationWindow(QWidget *parent = nullptr);
    virtual ~AnimationWindow() = default;

signals:
    void animationAdded();
    void animationDeleted(unsigned int index);
public slots:
    void onAnimationsSet();

private:
    AnimationList * m_animationList;
    AnimationPreview * m_animationPreview;
    AnimationTextureView * m_animationTextureView;

};

#endif // ANIMATIONWINDOW_H
