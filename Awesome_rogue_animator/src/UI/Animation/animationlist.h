#ifndef ANIMATIONLIST_H
#define ANIMATIONLIST_H

#include "animation.h"
#include "ressource.h"
#include <QWidget>
#include <QListWidget>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <vector>

class AnimationList : public QWidget
{
    Q_OBJECT
public:
    AnimationList(QWidget *parent = nullptr);
    virtual ~AnimationList() = default;

signals:
    void changeAnimation(Animation);
    void changeTexture(Texture);
    void animationAdded();
    void animationDeleted(unsigned int index);
    void animationRenamed(unsigned int index);

public slots:
    void frameChanged(unsigned int frameID, Frame f);
    void onFrameValueChange();
    void onAnimationListIndexChange(int);
    void onFrameListIndexChange(int newIndex);
    void onSingleShootChange();
    void onRightClickAnimations(QPoint point);
    void onRightClickFrames(QPoint point);
    void onSelectTexture();
    void onAnimationsSet();
    void onTextureLoad(Texture t);

private:
    int currentAnimationIndex() const;
    int currentFrameIndex() const;
    void updateFrameData(int animationID, int frameID);
    void updateAnimationList();
    void updateFrameList(bool notChangeIndex);
    void updateSingleShoot();

    Texture m_texture;

    QListWidget * m_animationList;
    QListWidget * m_frameList;
    QCheckBox * m_single;
    QDoubleSpinBox * m_timeWidget;
    QDoubleSpinBox * m_rectWidth;
    QDoubleSpinBox * m_rectHeight;
    QDoubleSpinBox * m_rectLeft;
    QDoubleSpinBox * m_rectTop;
    QDoubleSpinBox * m_offsetX;
    QDoubleSpinBox * m_offsetY;
    QPushButton * m_chooseTextureButton;
    QLineEdit * m_textureText;
};

#endif // ANIMATIONLIST_H
