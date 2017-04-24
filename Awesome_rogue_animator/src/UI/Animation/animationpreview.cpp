#include "animationpreview.h"
#include <SFML/Graphics/Sprite.hpp>
#include <limits>

AnimationPreview::AnimationPreview(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_backgroundColor(sf::Color::White)
{
    setMinimumHeight(100);
    setMinimumWidth(100);
}

void AnimationPreview::OnUpdate()
{
    RenderWindow::clear(m_backgroundColor);
    float time = m_clock.restart().asSeconds();
    m_currentTime += time;

    if(m_animation.frames.empty() || !m_texture.isValid())
        return;

    if(m_animation.ended(m_currentTime))
        m_currentTime = 0;

    Frame f(m_animation.at(m_currentTime));
    sf::Sprite s(*m_texture, sf::IntRect(f.texRect));
    s.setOrigin(f.offset);
    s.setPosition(0, 0);
    draw(s);
}

void AnimationPreview::onAnimationChange(Animation anim)
{
    m_animation = anim;
    m_currentTime = 0;
    updateRect();
    updateView();
}

void AnimationPreview::onTextureChange(Texture texture)
{
    m_texture = texture;
    m_currentTime = 0;
}

void AnimationPreview::updateRect()
{
    if(m_animation.frames.empty())
        return;

    float left(std::numeric_limits<float>::max());
    float top(std::numeric_limits<float>::max());
    float right(std::numeric_limits<float>::lowest());
    float bottom(std::numeric_limits<float>::lowest());

    for(const Frame & f : m_animation.frames)
    {
        if(left > -f.offset.x)
            left = -f.offset.x;
        if(top > -f.offset.y)
            top = -f.offset.y;
        if(right < f.texRect.width - f.offset.x)
            right = f.texRect.width - f.offset.x;
        if(bottom < f.texRect.height - f.offset.y)
            bottom = f.texRect.height - f.offset.y;
    }
    m_rect = sf::FloatRect(left, top, right - left, bottom - top);
}

void AnimationPreview::updateView()
{
    const float margin(5);
    sf::FloatRect rectWithMargin(m_rect.left - margin, m_rect.top - margin, m_rect.width + 2*margin, m_rect.height + 2*margin);
    auto windowSize(RenderWindow::getSize());
    if(rectWithMargin.width / windowSize.x < rectWithMargin.height / windowSize.y)
    {
        float width = rectWithMargin.height / windowSize.y  * windowSize.x;
        rectWithMargin.left += (rectWithMargin.width - width) / 2;
        rectWithMargin.width = width;
    }
    else
    {
        float height = rectWithMargin.width / windowSize.x  * windowSize.y;
        rectWithMargin.top += (rectWithMargin.height - height) / 2;
        rectWithMargin.height = height;
    }

    sf::View v(RenderWindow::getView());
    v.setSize(rectWithMargin.width, rectWithMargin.height);
    v.setCenter(rectWithMargin.left + rectWithMargin.width / 2, rectWithMargin.top + rectWithMargin.height / 2);
    RenderWindow::setView(v);
}

void AnimationPreview::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    updateView();
}

void AnimationPreview::setBackgroundColor(sf::Color color)
{
    m_backgroundColor = color;
}
