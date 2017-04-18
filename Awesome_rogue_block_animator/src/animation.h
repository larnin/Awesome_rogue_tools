#ifndef ANIMATION_H
#define ANIMATION_H

#include "rotation.h"
#include <vector>

struct Frame
{
    Frame(float _minTime, float _maxTime, unsigned int _id, Rotation _rot, bool _flipX, bool _flipY)
        : minTime(_minTime)
        , maxTime(_maxTime)
        , id(_id)
        , rot(_rot)
        , flipX(_flipX)
        , flipY(_flipY)
    {

    }

    Frame()
        : minTime(0.1)
        , maxTime(0.2)
        , id(0)
        , rot(ROT_0)
        , flipX(false)
        , flipY(false)
    {

    }

    float minTime;
    float maxTime;
    unsigned int id;
    Rotation rot;
    bool flipX;
    bool flipY;
};

struct SingleAnimation
{
    SingleAnimation(float _weight)
        : weight(_weight) {}

    float weight;
    std::vector<Frame> frames;
};


struct Animation
{
    Animation(unsigned int _id)
        : id(_id) {}

    unsigned int id;
    std::vector<SingleAnimation> anims;
};


#endif // ANIMATION_H
