#ifndef LIGHT_H
#define LIGHT_H

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <QJsonObject>
#include <string>
#include <vector>

enum LightType
{
    POINT,
    DIRECTIONNAL,
    SPOT,
};

std::string lightName(LightType type);

struct LightFrame
{
    LightFrame(const QJsonObject & obj);
    LightFrame(float _time = 1);

    QJsonObject toJson() const;

    float time;
    sf::Vector3f pos;
    sf::Color color;
    float radius;
    float yaw;
    float pitch;
};

class Light
{
public:
    Light(LightType type);
    Light(const QJsonObject obj);

    QJsonObject toJson() const;

    inline unsigned int frameCount() const { return m_frames.size(); }
    inline const LightFrame & operator[] (unsigned int index ) const { return m_frames[index]; }
    inline LightFrame & operator [] (unsigned int index) { return m_frames[index]; }

    inline std::vector<LightFrame>::const_iterator begin() const {return m_frames.begin(); }
    inline std::vector<LightFrame>::iterator begin() { return m_frames.begin(); }
    inline std::vector<LightFrame>::const_iterator end() const {return m_frames.end(); }
    inline std::vector<LightFrame>::iterator end() { return m_frames.end(); }

    float time() const;

    LightFrame at(float t) const;

    static LightFrame defaultFrame();

private:
    unsigned int frameIndex(float &t) const;

    static LightFrame interpolate(const LightFrame & f1, const LightFrame & f2, float lerp);

    template <typename T>
    static T interpolate(const T & a, const T & b, float lerp)
    {
        return a * (1-lerp) + b * lerp;
    }

    LightType m_type;
    std::vector<LightFrame> m_frames;
};

#endif // LIGHT_H
