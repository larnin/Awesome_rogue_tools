#include "light.h"
#include <QJsonArray>
#include <algorithm>
#include <cmath>

std::string lightName(LightType type)
{
    switch(type)
    {
    case LightType::POINT:
        return "Point";
    case LightType::SPOT:
        return "Spot";
    case LightType::DIRECTIONNAL:
        return "Directionnal";
    default:
        return "";
    }
}

LightFrame::LightFrame(float _time)
    : time(_time)
    , color(sf::Color::White)
    , radius(100)
    , yaw(0)
    , pitch(0)
    , intensity(0)
{

}

LightFrame::LightFrame(const QJsonObject & obj)
    : time(obj["time"].toDouble())
    , radius(obj["radius"].toDouble())
    , yaw(obj["yaw"].toDouble())
    , pitch(obj["pitch"].toDouble())
    , intensity(obj["intensity"].toDouble())
{
    auto posObj(obj["pos"].toObject());
    pos = sf::Vector3f(posObj["x"].toDouble(), posObj["y"].toDouble(), posObj["z"].toDouble());

    auto colorObj(obj["color"].toObject());
    color = sf::Color(colorObj["r"].toInt(), colorObj["g"].toInt(), colorObj["b"].toInt(), colorObj["a"].toInt());
}

QJsonObject LightFrame::toJson() const
{
    QJsonObject obj;
    obj.insert("time", time);
    obj.insert("radius", radius);
    obj.insert("yaw", yaw);
    obj.insert("pitch", pitch);
    obj.insert("intensity", intensity);
    QJsonObject posObj;
    posObj.insert("x", pos.x);
    posObj.insert("y", pos.y);
    posObj.insert("z", pos.z);
    obj.insert("pos", posObj);
    QJsonObject colorObj;
    colorObj.insert("r", color.r);
    colorObj.insert("g", color.g);
    colorObj.insert("b", color.b);
    colorObj.insert("a", color.a);
    obj.insert("color", colorObj);
    return obj;
}

Light::Light(LightType type)
    : m_type(type)
{

}

Light::Light(const QJsonObject obj)
    : m_type(LightType(obj["type"].toInt()))
{
    auto array(obj["frames"].toArray());
    for(const auto & v : array)
        m_frames.emplace_back(v.toObject());
}

QJsonObject Light::toJson() const
{
    QJsonObject obj;
    obj.insert("type", int(m_type));
    QJsonArray array;
    for(const auto & v : m_frames)
        array.append(v.toJson());
    obj.insert("frames", array);
    return obj;
}

float Light::time() const
{
    return std::accumulate(m_frames.begin(), m_frames.end(), 0.0f, [](float a, const LightFrame & b){return a + b.time;});
}

void Light::add(const LightFrame & f)
{
    m_frames.push_back(f);
}

void Light::del(unsigned int index)
{
    if(index >= m_frames.size())
        return;
    m_frames.erase(std::next(m_frames.begin(), index));
}

LightFrame Light::at(float t) const
{
    if(m_frames.empty())
        return defaultFrame();
    t = fmod(t, time());

    auto index = frameIndex(t);
    auto nextIndex = index >= m_frames.size()-1 ? 0 : index + 1;
    return interpolate(m_frames[index], m_frames[nextIndex], t / m_frames[index].time);
}

LightFrame Light::defaultFrame()
{
    return LightFrame(1);
}

unsigned int Light::frameIndex(float & t) const
{
    float totalTime(0);
    for(unsigned int i(0) ; i < m_frames.size() ; i++)
    {
        totalTime += m_frames[i].time;
        if(totalTime > t)
        {
            t = totalTime - t;
            return i;
        }
    }

    t = 0;
    return m_frames.size() - 1;
}

LightFrame Light::interpolate(const LightFrame & f1, const LightFrame &f2, float lerp)
{
    LightFrame f(f1.time * lerp);
    f.color = sf::Color(interpolate(f1.color.r, f2.color.r, lerp)
                      , interpolate(f1.color.g, f2.color.g, lerp)
                      , interpolate(f1.color.b, f2.color.b, lerp)
                      , interpolate(f1.color.a, f2.color.a, lerp));
    f.pitch = interpolate(f1.pitch, f2.pitch, lerp);
    f.pos = sf::Vector3f(interpolate(f1.pos.x, f2.pos.x, lerp)
                       , interpolate(f1.pos.y, f2.pos.y, lerp)
                       , interpolate(f1.pos.z, f2.pos.z, lerp));
    f.radius = interpolate(f1.radius, f2.radius, lerp);
    f.yaw = interpolate(f1.yaw, f2.yaw, lerp);
    f.intensity = interpolate(f1.intensity, f2.intensity, lerp);
    return f;
}
