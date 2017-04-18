#ifndef ANIMATIONTEXTUREVIEW_H
#define ANIMATIONTEXTUREVIEW_H

#include "ressource.h"
#include "animation.h"
#include "qsfmlcanvas.h"
#include <SFML/System/Vector2.hpp>

class AnimationTextureView : public QSFMLCanvas
{
    Q_OBJECT
public:
    AnimationTextureView(QWidget * parent = nullptr);
    virtual ~AnimationTextureView() = default;

signals:
    void changeFrame(unsigned int frameID, Frame f);

public slots:
    void onAnimationChange(Animation anim);
    void onTextureChange(Texture texture);
    void setBackgroundColor(sf::Color color);

protected:
    virtual void OnUpdate() override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    void updateView();

    Animation m_animation;
    Texture m_texture;
    sf::Color m_backgroundColor;
    int m_zoomLevel;
    sf::Vector2f m_center;
    sf::Vector2f m_oldMousePos;
};

inline float zoomFromLevel(int level)
{
    return std::pow(std::sqrt(2), level);
}

#endif // ANIMATIONTEXTUREVIEW_H
