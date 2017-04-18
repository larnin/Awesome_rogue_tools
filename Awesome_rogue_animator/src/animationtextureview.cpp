#include "animationtextureview.h"
#include <SFML/Graphics/Sprite.hpp>
#include <QWheelEvent>

AnimationTextureView::AnimationTextureView(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_backgroundColor(sf::Color::White)
    , m_zoomLevel(0)
    , m_center(0, 0)
{
    setMinimumHeight(100);
    setMinimumWidth(100);
    setMouseTracking(true);
}


void AnimationTextureView::OnUpdate()
{
    RenderWindow::clear(m_backgroundColor);

    if(!m_texture.isValid())
        return;

    sf::Sprite s(*m_texture);
    RenderWindow::draw(s);
}

void AnimationTextureView::onAnimationChange(Animation anim)
{
    m_animation = anim;
}

void AnimationTextureView::onTextureChange(Texture texture)
{
    m_texture = texture;
    m_zoomLevel = 0;
    if(m_texture.isValid())
    {
        m_center = sf::Vector2f(m_texture->getSize().x/2, m_texture->getSize().y/2);
        updateView();
    }
}

void AnimationTextureView::setBackgroundColor(sf::Color color)
{
    m_backgroundColor = color;
}

void AnimationTextureView::updateView()
{
    sf::View v(RenderWindow::getView());
    auto windowSize(RenderWindow::getSize());
    float zoom(zoomFromLevel(m_zoomLevel));
    v.setSize(windowSize.x*zoom, windowSize.y*zoom);
    v.setCenter(m_center);
    RenderWindow::setView(v);
}

void AnimationTextureView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
        m_zoomLevel ++;
    if(event->angleDelta().y() < 0)
        m_zoomLevel --;
    if(m_zoomLevel < -10)
        m_zoomLevel = -10;
    if(m_zoomLevel > 10)
        m_zoomLevel = 10;
    updateView();
}

void AnimationTextureView::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    updateView();
}

void AnimationTextureView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons().testFlag(Qt::LeftButton))
    {
        float zoom(zoomFromLevel(m_zoomLevel));
        m_center.x -= (event->pos().x() - m_oldMousePos.x) * zoom;
        m_center.y -= (event->pos().y() - m_oldMousePos.y) * zoom;
        updateView();
    }
    m_oldMousePos = sf::Vector2f(event->pos().x(), event->pos().y());
}
