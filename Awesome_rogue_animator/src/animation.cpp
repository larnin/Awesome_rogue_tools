#include "animation.h"
#include "QJsonValue"
#include "QJsonarray.h"

Frame Animation::at(float time) const
{
    float total(0);
    for(const Frame & f : frames)
    {
        total += f.time;
        if(total >= time)
            return f;
    }
    if(!frames.empty())
        return frames.back();
    return Frame();
}

bool Animation::ended(float time) const
{
    float total(0);
    for(const Frame & f : frames)
        total += f.time;
    return total <= time;
}

QJsonObject toJson(const Animation & a)
{
    QJsonObject obj;
    obj.insert("singleShoot", a.singleShoot);
    obj.insert("name", QString::fromStdString(a.name));
    QJsonArray array;
    for(const Frame & f : a.frames)
    {
        QJsonObject fObj;
        fObj.insert("time", f.time);
        fObj.insert("texX", f.texRect.left);
        fObj.insert("texY", f.texRect.top);
        fObj.insert("texH", f.texRect.height);
        fObj.insert("texW", f.texRect.width);
        fObj.insert("offsetX", f.offset.x);
        fObj.insert("offsetY", f.offset.y);
        array.append(fObj);
    }
    obj.insert("frames", array);
    return obj;
}

Animation toAnimation(const QJsonObject & j)
{
    Animation a;
    a.name = j["name"].toString().toStdString();
    a.singleShoot = j["singleShoot"].toBool();
    auto frames = j["frames"];
    if(frames.isArray())
    {
        for(const auto & value : frames.toArray())
        {
            if(!value.isObject())
                continue;
            auto frameObj = value.toObject();
            Frame f;
            f.time = frameObj["time"].toDouble();
            f.texRect.left = frameObj["texX"].toDouble();
            f.texRect.top = frameObj["texY"].toDouble();
            f.texRect.width = frameObj["texW"].toDouble();
            f.texRect.height = frameObj["texH"].toDouble();
            f.offset.x = frameObj["offsetX"].toDouble();
            f.offset.y = frameObj["offsetY"].toDouble();
            a.frames.push_back(f);
        }
    }

    return a;
}
