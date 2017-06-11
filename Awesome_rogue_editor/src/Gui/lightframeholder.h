#ifndef LIGHTFRAMEHOLDER_H
#define LIGHTFRAMEHOLDER_H

#include "Map/light.h"
#include <QWidget>
#include <QDoubleSpinBox>
#include <QPushButton>

class FramePointLightHolder : public QWidget
{
    Q_OBJECT

public:
    FramePointLightHolder(LightFrame & frame, QWidget * parent = nullptr);

private:
    LightFrame & m_frame;

    QDoubleSpinBox* m_timeWidget;
    QDoubleSpinBox* m_xWidget;
    QDoubleSpinBox* m_yWidget;
    QDoubleSpinBox* m_zWidget;
    QPushButton* m_colorPicker;
    QDoubleSpinBox* m_radiusWidget;
    QDoubleSpinBox* m_intensityWidget;
};

class FrameDirectionnalLightHolder : public QWidget
{
    Q_OBJECT

public:
    FrameDirectionnalLightHolder(LightFrame & frame, QWidget * parent = nullptr);

private:
    LightFrame & m_frame;

    QDoubleSpinBox* m_timeWidget;
    QPushButton* m_colorPicker;
    QDoubleSpinBox* m_radiusWidget;
    QDoubleSpinBox* m_intensityWidget;
    QDoubleSpinBox* m_yawWidget;
    QDoubleSpinBox* m_pitchWidget;
};

class FrameSpotLightHolder : public QWidget
{
    Q_OBJECT

public:
    FrameSpotLightHolder(LightFrame & frame, QWidget * parent = nullptr);

private:
    LightFrame & m_frame;

    QDoubleSpinBox* m_timeWidget;
    QDoubleSpinBox* m_xWidget;
    QDoubleSpinBox* m_yWidget;
    QDoubleSpinBox* m_zWidget;
    QPushButton* m_colorPicker;
    QDoubleSpinBox* m_radiusWidget;
    QDoubleSpinBox* m_intensityWidget;
    QDoubleSpinBox* m_yaw;
    QDoubleSpinBox* m_angle;
};

#endif // LIGHTFRAMEHOLDER_H

/*
QPixmap pixmap(16, 16);
pixmap.fill(color);
label->setPixmap(pixmap);
*/
