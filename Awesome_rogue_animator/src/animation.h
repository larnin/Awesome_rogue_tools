#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"
#include <QJsonObject>
#include <vector>
#include <string>

struct Animation
{
    inline Animation()
        : singleShoot(false) {}
    bool singleShoot;
    std::vector<Frame> frames;
    std::string name;

    Frame at(float time) const;
    bool ended(float time) const;
};

QJsonObject toJson(const Animation & a);
Animation toAnimation(const QJsonObject & j);

#endif // ANIMATION_H
