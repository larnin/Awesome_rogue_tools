#include "lightframeholder.h"

FramePointLightHolder::FramePointLightHolder(LightFrame & frame, QWidget * parent)
    : QWidget(parent)
    , m_frame(frame)
{

}

FrameDirectionnalLightHolder::FrameDirectionnalLightHolder(LightFrame & frame, QWidget * parent)
    : QWidget(parent)
    , m_frame(frame)
{

}

FrameSpotLightHolder::FrameSpotLightHolder(LightFrame & frame, QWidget * parent)
    : QWidget(parent)
    , m_frame(frame)
{

}
