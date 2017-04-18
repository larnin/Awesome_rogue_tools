#include "animationviewer.h"
#include "tilesetdata.h"
#include <cassert>
#include "quadrender.h"
#include "tile.h"

AnimationViewer::AnimationViewer(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_id(-1)
    , m_anim(-1)
{

}

void AnimationViewer::OnUpdate()
{
    RenderWindow::clear(sf::Color::Black);

    sf::View v(RenderWindow::getView());
    v.setCenter(0, 0);
    v.setSize(sf::Vector2f(70/64.0f, 70/64.0f));
    RenderWindow::setView(v);

    float time(m_clock.restart().asSeconds());
    m_animation.currentTime += time;
    if(!m_animation.valid)
        return;

    if(!TilesetData::instance.texture.isValid())
        return;

    sf::VertexArray array(sf::Quads, 4);
    Frame f(m_animation.current());
    drawQuad(&array[0], sf::FloatRect(-0.5f, -0.5f, 1, 1), texRect(f.id, TilesetData::instance.tileSize, TilesetData::instance.texture->getSize())
            , f.flipX, f.flipY, f.rot);

    RenderWindow::draw(array, sf::RenderStates(TilesetData::instance.texture()));

    if(m_animation.finished())
        m_animation = createAnimation(m_id, m_anim);
}

void AnimationViewer::setAnimationIndex(int ID, int anim)
{
    m_animation = createAnimation(ID, anim-1);
    m_id = ID;
    m_anim = anim-1;

}

AnimationViewer::PlayedAnimation AnimationViewer::createAnimation(int id, int anim)
{
    PlayedAnimation a;
    if(id < 0)
        return a;

    auto animations(TilesetData::instance.animations);

    auto it(std::find_if(animations.begin(), animations.end(), [id](const auto & a){return int(a.id) == id;}));
    if(it == animations.end())
        return a;

    if(anim < 0)
    {
        std::vector<float> poids;
        for(const auto & a : it->anims)
            poids.push_back(a.frames.empty() ? 0 : a.weight);
        anim = std::discrete_distribution<unsigned int>(poids.begin(), poids.end())(m_engine);
        //anim = std::uniform_int_distribution<unsigned int>(0, it->anims.size()-1)(m_engine);

    }
    if(anim >= int(it->anims.size()))
        return a;

    a.valid = true;

    for(const Frame & f : it->anims[anim].frames)
    {
        float time(std::uniform_real_distribution<float>(std::min(f.minTime, f.maxTime), std::max(f.minTime, f.maxTime))(m_engine));
        a.animation.frames.push_back(Frame(time, time, f.id, f.rot, f.flipX, f.flipY));
    }

    return a;
}

bool AnimationViewer::PlayedAnimation::finished() const
{
    float totalTime(std::accumulate(animation.frames.begin(), animation.frames.end(), 0.0f,
                                    [](float t, const auto & f){return t+f.minTime;}));
    return totalTime <= currentTime;
}

Frame AnimationViewer::PlayedAnimation::current() const
{
    assert(!animation.frames.empty());

    float time(currentTime);
    for(const Frame & f : animation.frames)
    {
        time -= f.minTime;
        if(time <= 0)
            return f;
    }
    return animation.frames.back();
}
