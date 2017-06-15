#include "lightframeholder.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QGroupBox>
#include <QColorDialog>

const float radToDeg = 180 / 3.14159f;
const float degToRad = 1 / radToDeg;

void setSpinBox(QDoubleSpinBox & box, double min, double max, int decimal, double step, double value)
{
    box.setRange(min, max);
    box.setDecimals(decimal);
    box.setSingleStep(step);
    box.setValue(value);
}

QIcon generateColor(const sf::Color & color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(QColor(color.r, color.g, color.b, color.a));
    return QIcon(pixmap);
}

FramePointLightHolder::FramePointLightHolder(LightFrame & frame, QWidget * parent)
    : QWidget(parent)
    , m_frame(frame)
{
    m_timeWidget = new QDoubleSpinBox();
    setSpinBox(*m_timeWidget, 0, 100, 3, 0.01, frame.time);
    m_xWidget = new QDoubleSpinBox();
    setSpinBox(*m_xWidget, -100, 1000, 2, 0.1, frame.pos.x);
    m_yWidget = new QDoubleSpinBox();
    setSpinBox(*m_yWidget, -100, 1000, 2, 0.1, frame.pos.y);
    m_zWidget = new QDoubleSpinBox();
    setSpinBox(*m_zWidget, -100, 1000, 2, 0.1, frame.pos.z);
    m_radiusWidget = new QDoubleSpinBox();
    setSpinBox(*m_radiusWidget, 0, 500, 2, 0.1, frame.radius);
    m_intensityWidget = new QDoubleSpinBox();
    setSpinBox(*m_intensityWidget, 0, 10, 2, 0.1, frame.intensity);
    m_colorPicker = new QPushButton(generateColor(frame.color), "");
    m_colorPicker->setFixedWidth(25);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addWidget(new QLabel("Temps : "));
    timeLayout->addWidget(m_timeWidget, 1);

    QHBoxLayout *posLayout = new QHBoxLayout();
    posLayout->addWidget(new QLabel("X : "));
    posLayout->addWidget(m_xWidget, 1);
    posLayout->addWidget(new QLabel("Y :"));
    posLayout->addWidget(m_yWidget, 1);

    QHBoxLayout *heightLayout = new QHBoxLayout();
    heightLayout->addWidget(new QLabel("Height :"));
    heightLayout->addWidget(m_zWidget, 1);

    QHBoxLayout *radiusLayout = new QHBoxLayout();
    radiusLayout->addWidget(new QLabel("Rayon : "));
    radiusLayout->addWidget(m_radiusWidget, 1);

    QHBoxLayout *colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Couleur : "));
    colorLayout->addWidget(m_colorPicker);

    QHBoxLayout *intensityLayout = new QHBoxLayout();
    intensityLayout->addWidget(new QLabel("Intensité : "));
    intensityLayout->addWidget(m_intensityWidget, 1);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(timeLayout);
    layout->addLayout(posLayout);
    layout->addLayout(heightLayout);
    layout->addLayout(radiusLayout);
    layout->addLayout(colorLayout);
    layout->addLayout(intensityLayout);

    setLayout(layout);

    connect(m_colorPicker, SIGNAL(clicked(bool)), this, SLOT(onPickColor()));
    connect(m_timeWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_xWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_yWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_zWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_radiusWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_intensityWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
}

void FramePointLightHolder::onValueChanged()
{
    m_frame.time = m_timeWidget->value();
    m_frame.pos = sf::Vector3f(m_xWidget->value(), m_yWidget->value(), m_zWidget->value());
    m_frame.radius = m_radiusWidget->value();
    m_frame.intensity = m_intensityWidget->value();
}

void FramePointLightHolder::onPickColor()
{
    QColor c (QColorDialog::getColor(QColor(m_frame.color.r, m_frame.color.g, m_frame.color.b), this, "Couleur de la frame"));
    if(!c.isValid())
        return;

    m_frame.color = sf::Color(c.red(), c.green(), c.blue());
    m_colorPicker->setIcon(generateColor(m_frame.color));
}

// ----------------------------------------------

FrameDirectionnalLightHolder::FrameDirectionnalLightHolder(LightFrame & frame, QWidget * parent)
    : QWidget(parent)
    , m_frame(frame)
{
    m_timeWidget = new QDoubleSpinBox();
    setSpinBox(*m_timeWidget, 0, 100, 3, 0.01, frame.time);
    m_colorPicker = new QPushButton(generateColor(frame.color), "");
    m_colorPicker->setFixedWidth(25);
    m_radiusWidget = new QDoubleSpinBox();
    setSpinBox(*m_radiusWidget, 0, 500, 2, 0.1, frame.radius);
    m_intensityWidget = new QDoubleSpinBox();
    setSpinBox(*m_intensityWidget, 0, 10, 2, 0.1, frame.intensity);
    m_yawWidget = new QDoubleSpinBox();
    setSpinBox(*m_yawWidget, -1000, 1000, 1, 1, frame.yaw * radToDeg);
    m_yawWidget->setMinimumWidth(50);
    m_pitchWidget = new QDoubleSpinBox();
    setSpinBox(*m_pitchWidget, -1000, 1000, 1, 1, frame.pitch * radToDeg);
    m_pitchWidget->setMinimumWidth(50);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addWidget(new QLabel("Temps : "));
    timeLayout->addWidget(m_timeWidget, 1);

    QHBoxLayout *radiusLayout = new QHBoxLayout();
    radiusLayout->addWidget(new QLabel("Rayon : "));
    radiusLayout->addWidget(m_radiusWidget, 1);

    QHBoxLayout *colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Couleur : "));
    colorLayout->addWidget(m_colorPicker);

    QHBoxLayout *intensityLayout = new QHBoxLayout();
    intensityLayout->addWidget(new QLabel("Intensité : "));
    intensityLayout->addWidget(m_intensityWidget, 1);

    QHBoxLayout *rotLayout = new QHBoxLayout();
    rotLayout->addWidget(new QLabel("Yaw :"));
    rotLayout->addWidget(m_yawWidget, 1);
    rotLayout->addWidget(new QLabel("Pitch :"));
    rotLayout->addWidget(m_pitchWidget, 1);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(timeLayout);
    layout->addLayout(radiusLayout);
    layout->addLayout(colorLayout);
    layout->addLayout(intensityLayout);
    layout->addLayout(rotLayout);

    setLayout(layout);

    connect(m_colorPicker, SIGNAL(clicked(bool)), this, SLOT(onPickColor()));
    connect(m_timeWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_radiusWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_intensityWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_yawWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_pitchWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
}

void FrameDirectionnalLightHolder::onValueChanged()
{
    m_frame.time = m_timeWidget->value();
    m_frame.radius = m_radiusWidget->value();
    m_frame.intensity = m_intensityWidget->value();
    m_frame.yaw = m_yawWidget->value() * degToRad;
    m_frame.pitch = m_pitchWidget->value() * degToRad;
}

void FrameDirectionnalLightHolder::onPickColor()
{
    QColor c (QColorDialog::getColor(QColor(m_frame.color.r, m_frame.color.g, m_frame.color.b), this, "Couleur de la frame"));
    if(!c.isValid())
        return;

    m_frame.color = sf::Color(c.red(), c.green(), c.blue());
    m_colorPicker->setIcon(generateColor(m_frame.color));
}

// ----------------------------------------------

FrameSpotLightHolder::FrameSpotLightHolder(LightFrame & frame, QWidget * parent)
    : QWidget(parent)
    , m_frame(frame)
{
    m_timeWidget = new QDoubleSpinBox();
    setSpinBox(*m_timeWidget, 0, 100, 3, 0.01, frame.time);
    m_xWidget = new QDoubleSpinBox();
    setSpinBox(*m_xWidget, -100, 1000, 2, 0.1, frame.pos.x);
    m_yWidget = new QDoubleSpinBox();
    setSpinBox(*m_yWidget, -100, 1000, 2, 0.1, frame.pos.y);
    m_zWidget = new QDoubleSpinBox();
    setSpinBox(*m_zWidget, -100, 1000, 2, 0.1, frame.pos.z);
    m_radiusWidget = new QDoubleSpinBox();
    setSpinBox(*m_radiusWidget, 0, 500, 2, 0.1, frame.radius);
    m_intensityWidget = new QDoubleSpinBox();
    setSpinBox(*m_intensityWidget, 0, 10, 2, 0.1, frame.intensity);
    m_colorPicker = new QPushButton(generateColor(frame.color), "");
    m_colorPicker->setFixedWidth(25);
    m_yawWidget = new QDoubleSpinBox();
    setSpinBox(*m_yawWidget, -1000, 1000, 1, 1, frame.yaw * radToDeg);
    m_yawWidget->setMinimumWidth(50);
    m_angleWidget = new QDoubleSpinBox();
    setSpinBox(*m_angleWidget, -1000, 1000, 1, 1, frame.pitch * radToDeg);
    m_angleWidget->setMinimumWidth(50);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addWidget(new QLabel("Temps : "));
    timeLayout->addWidget(m_timeWidget, 1);

    QHBoxLayout *posLayout = new QHBoxLayout();
    posLayout->addWidget(new QLabel("X : "));
    posLayout->addWidget(m_xWidget, 1);
    posLayout->addWidget(new QLabel("Y :"));
    posLayout->addWidget(m_yWidget, 1);

    QHBoxLayout *heightLayout = new QHBoxLayout();
    heightLayout->addWidget(new QLabel("Height :"));
    heightLayout->addWidget(m_zWidget, 1);

    QHBoxLayout *radiusLayout = new QHBoxLayout();
    radiusLayout->addWidget(new QLabel("Rayon : "));
    radiusLayout->addWidget(m_radiusWidget, 1);

    QHBoxLayout *colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Couleur : "));
    colorLayout->addWidget(m_colorPicker);

    QHBoxLayout *intensityLayout = new QHBoxLayout();
    intensityLayout->addWidget(new QLabel("Intensité : "));
    intensityLayout->addWidget(m_intensityWidget, 1);

    QHBoxLayout *rotLayout = new QHBoxLayout();
    rotLayout->addWidget(new QLabel("Yaw :"));
    rotLayout->addWidget(m_yawWidget, 1);
    rotLayout->addWidget(new QLabel("Angle :"));
    rotLayout->addWidget(m_angleWidget, 1);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(timeLayout);
    layout->addLayout(posLayout);
    layout->addLayout(heightLayout);
    layout->addLayout(radiusLayout);
    layout->addLayout(colorLayout);
    layout->addLayout(intensityLayout);
    layout->addLayout(rotLayout);

    setLayout(layout);

    connect(m_colorPicker, SIGNAL(clicked(bool)), this, SLOT(onPickColor()));
    connect(m_timeWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_xWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_yWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_zWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_radiusWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_intensityWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_yawWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(m_angleWidget, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
}

void FrameSpotLightHolder::onValueChanged()
{
    m_frame.time = m_timeWidget->value();
    m_frame.pos = sf::Vector3f(m_xWidget->value(), m_yWidget->value(), m_zWidget->value());
    m_frame.radius = m_radiusWidget->value();
    m_frame.intensity = m_intensityWidget->value();
    m_frame.yaw = m_yawWidget->value() * degToRad;
    m_frame.pitch = m_angleWidget->value() * degToRad;
}

void FrameSpotLightHolder::onPickColor()
{
    QColor c (QColorDialog::getColor(QColor(m_frame.color.r, m_frame.color.g, m_frame.color.b), this, "Couleur de la frame"));
    if(!c.isValid())
        return;

    m_frame.color = sf::Color(c.red(), c.green(), c.blue());
    m_colorPicker->setIcon(generateColor(m_frame.color));
}
