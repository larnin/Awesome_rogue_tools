#ifndef ANIMATIONVIEWER_H
#define ANIMATIONVIEWER_H

#include "qsfmlcanvas.h"
#include <SFML/System/Clock.hpp>
#include "animation.h"
#include <random>

class AnimationViewer : public QSFMLCanvas
{
    Q_OBJECT

public:
    AnimationViewer(QWidget * parent = nullptr);
    virtual ~AnimationViewer() = default;

public slots:
    void setAnimationIndex(int ID, int anim);

protected:
    virtual void OnUpdate();

private:
    struct PlayedAnimation
    {
        PlayedAnimation()
            : animation(0)
            , currentTime(0)
            , valid(false)
        {}

        bool finished() const;
        Frame current() const;

        SingleAnimation animation;
        float currentTime;
        bool valid;
    };

    PlayedAnimation createAnimation(int id, int anim);

    sf::Clock m_clock;
    PlayedAnimation m_animation;
    int m_id;
    int m_anim;
    std::default_random_engine m_engine;
};

#endif // ANIMATIONVIEWER_H
