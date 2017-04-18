#ifndef ANIMATIONPREVIEW_H
#define ANIMATIONPREVIEW_H

#include "qsfmlcanvas.h"
#include "animation.h"
#include "ressource.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>

class AnimationPreview : public QSFMLCanvas
{
    Q_OBJECT
public:
    AnimationPreview(QWidget * parent = nullptr);
    virtual ~AnimationPreview() = default;

public slots:
    void onAnimationChange(Animation anim);
    void onTextureChange(Texture texture);
    void setBackgroundColor(sf::Color color);

protected:
    virtual void OnUpdate() override;
    virtual void resizeEvent(QResizeEvent * event) override;

private:
    void updateRect();
    void updateView();

    Animation m_animation;
    Texture m_texture;
    float m_currentTime;
    sf::Clock m_clock;
    sf::FloatRect m_rect;
    sf::Color m_backgroundColor;
};

#endif // ANIMATIONPREVIEW_H
