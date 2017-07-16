#include "roomrender.h"
#include "Utilities/quadrender.h"
#include "blocktype.h"
#include "Utilities/configs.h"
#include "Utilities/vect2convert.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Glsl.hpp>

RoomRender::RoomRender(std::weak_ptr<Patern> room, bool showLights)
    : m_render(sf::Quads)
    , m_room(room)
    , m_showWallsBoxs(false)
    , m_wallsRender(sf::Lines)
    , m_showLights(showLights)
{
    redraw();

    m_phong = std::make_unique<sf::Shader>();
    m_phong->loadFromFile("res/shader/phong/2dmultilight.vert", "res/shader/phong/2dmultilight.frag");
}

void RoomRender::redraw()
{
    if(!Configs::tiles.texture.isValid())
        return;

    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
    {
        m_render.resize(0);
        return;
    }
    m_render.resize(getNbSurfaces()*4);

    int index(0);
    for(unsigned int i(0) ; i < r->getSize().x ; i++)
        for(unsigned int j(0) ; j < r->getSize().y ; j++)
        {
            const Block &b((*r)(sf::Vector2u(i, j)));
            index += drawBlock(&m_render[index*4], b, sf::Vector2i(i, j));
        }

    if(m_showWallsBoxs)
        redrawWalls();
}

void RoomRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    updateShader();
    std::shared_ptr<Patern> r(m_room.lock());
    if(r)
    {
        sf::RectangleShape sharp(sf::Vector2f(r->getSize())*float(Configs::tiles.tileSize));
        sharp.setFillColor(sf::Color(255, 255, 255, 50));
        sharp.move(Configs::tiles.tileSize/-2.0f, Configs::tiles.tileSize/-2.0f);
        target.draw(sharp);

        if(Configs::tiles.texture.isValid())
        {
            sf::RenderStates states(Configs::tiles.texture());
            if(m_showLights)
                states.shader = m_phong.get();
            target.draw(m_render, states);
        }

        if(m_showWallsBoxs)
        {
            target.draw(m_wallsRender);
            target.draw(createLightsGismos());
        }
    }
}

unsigned int RoomRender::getNbSurfaces() const
{
    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
        return 0;

    int nbDrawable(0);
    for(unsigned int i(0) ; i < r->getSize().x ; i++)
        for(unsigned int j(0) ; j < r->getSize().y ; j++)
        {
            const Block & b((*r)(sf::Vector2u(i, j)));
            if(b.groundID != 0)
                nbDrawable++;
            if(b.wallID != 0)
                nbDrawable++;
            if(b.topID != 0)
                nbDrawable++;
        }
    return nbDrawable;
}

unsigned int RoomRender::drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const
{
    unsigned int offset(0);

    float tileSize(Configs::tiles.tileSize);

    if(b.groundID != 0)
    {
        sf::FloatRect rect(BlockType::texRect(b.groundID, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
        drawQuad(quads, sf::FloatRect(sf::Vector2f(globalPos)*tileSize-sf::Vector2f(tileSize, tileSize)/2.0f, sf::Vector2f(tileSize, tileSize))
                 , rect, getXFlip(b.groundOrientation), getYFlip(b.groundOrientation), getRotation(b.groundOrientation));
        offset++;
    }
    if(b.wallID != 0)
    {
        sf::FloatRect rect(BlockType::texRect(b.wallID, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
        drawQuad(quads+4*offset, sf::FloatRect(sf::Vector2f(globalPos)*tileSize-sf::Vector2f(tileSize, tileSize)/2.0f, sf::Vector2f(tileSize, tileSize))
                 , rect, getXFlip(b.wallOrientation), getYFlip(b.wallOrientation), getRotation(b.wallOrientation));
        offset++;
    }
    if(b.topID != 0)
    {
        sf::FloatRect rect(BlockType::texRect(b.topID, Configs::tiles.tileSize, Configs::tiles.texture->getSize()));
        drawQuad(quads+4*offset, sf::FloatRect(sf::Vector2f(globalPos)*tileSize-sf::Vector2f(tileSize, tileSize)/2.0f, sf::Vector2f(tileSize, tileSize))
                 , rect, getXFlip(b.topOrientation), getYFlip(b.topOrientation), getRotation(b.topOrientation));
        offset++;
    }

    return offset;
}

std::weak_ptr<Patern> RoomRender::getRoom() const
{
    return m_room;
}

void RoomRender::showWallsBoxs(bool value)
{
    m_showWallsBoxs = value;
    if(!value)
        m_wallsRender.resize(0);
    else redrawWalls();
}

void RoomRender::redrawWalls()
{
    m_wallsRender.clear();

    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
        return;

    for(unsigned int i(0) ; i < r->getSize().x ; i++)
        for(unsigned int j(0) ; j < r->getSize().y ; j++)
        {
            const Block &b((*r)(sf::Vector2u(i, j)));
            HitBox box(BlockType::createBox(b.boxCaracts));
            for(const Line & l : box.lines)
            {
                m_wallsRender.append(sf::Vertex((l.pos1+sf::Vector2f(i, j))*float(Configs::tiles.tileSize), sf::Color::Yellow));
                m_wallsRender.append(sf::Vertex((l.pos2+sf::Vector2f(i, j))*float(Configs::tiles.tileSize), sf::Color::Yellow));
            }
        }
}

void RoomRender::updateShader() const
{
    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
        return;

    using sf::Glsl::Vec4;
    using sf::Glsl::Vec3;

    Vec4 material(Configs::tiles.material.ambiantCoeficient
                , Configs::tiles.material.diffuseCoefficient
                , Configs::tiles.material.specularCoefficient
                , Configs::tiles.material.specularMultiplier);
    m_phong->setUniform("material", material);
    m_phong->setUniform("lightCount", (int)r->lightCount());
    m_phong->setUniform("primaryTexture", *Configs::tiles.texture);
    m_phong->setUniform("secondaryTexture", *Configs::tiles.material.normal);
    m_phong->setUniform("ambiantColor", Vec4(Configs::tiles.ambiantColor));

    std::vector<Vec4> lightsColor;
    std::vector<Vec3> lightsPos;
    std::vector<float> lightsType;
    std::vector<Vec4> lightsParams;

    float time(m_clock.getElapsedTime().asSeconds());

    for(unsigned int i(0) ; i < r->lightCount() ; i++)
    {
        const auto & l(r->light(i));
        const auto & f(l.at(time));

        lightsType.push_back(l.type());
        lightsColor.push_back(Vec4(f.color));
        lightsPos.push_back(f.pos);
        lightsParams.push_back(Vec4(f.radius, f.intensity, f.yaw, f.pitch));
    }

    m_phong->setUniformArray("lightColor", lightsColor.data(), lightsColor.size());
    m_phong->setUniformArray("light", lightsPos.data(), lightsPos.size());
    m_phong->setUniformArray("lightType", lightsType.data(), lightsType.size());
    m_phong->setUniformArray("lightParams", lightsParams.data(), lightsParams.size());
}

namespace
{
void drawPoint(sf::VertexArray & array, const LightFrame & frame)
{
    sf::Vector2f pos(frame.pos.x, frame.pos.y);
    array.append(sf::Vertex(pos - sf::Vector2f(-2, -2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(2, 2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(-2, 2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(2, -2), frame.color));

    const unsigned int count = 50;
    for(unsigned int i(0) ; i < count ; i++)
    {
        array.append(sf::Vertex(pos + toVect(frame.radius, float(M_PI)/count * i * 2), frame.color));
        array.append(sf::Vertex(pos + toVect(frame.radius, float(M_PI)/count * (i+1) * 2), frame.color));
    }
}

void drawSpot(sf::VertexArray & array, const LightFrame & frame)
{
    sf::Vector2f pos(frame.pos.x, frame.pos.y);
    array.append(sf::Vertex(pos - sf::Vector2f(-2, -2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(2, 2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(-2, 2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(2, -2), frame.color));

    float startAngle = frame.yaw - frame.pitch;
    float endAngle = frame.yaw + frame.pitch;

    array.append(sf::Vertex(pos, frame.color));
    array.append(sf::Vertex(pos + toVect(frame.radius, startAngle), frame.color));
    array.append(sf::Vertex(pos, frame.color));
    array.append(sf::Vertex(pos + toVect(frame.radius, endAngle), frame.color));

    const unsigned int count = 50;
    const float delta(float(M_PI)/count * 2);
    for(float i(startAngle) ; i < endAngle ; i += delta)
    {
        float end = std::min(i + delta, endAngle);

        array.append(sf::Vertex(pos + toVect(frame.radius, i), frame.color));
        array.append(sf::Vertex(pos + toVect(frame.radius, end), frame.color));
    }
}

void drawDirectional(sf::VertexArray & array, const LightFrame & frame)
{
    sf::Vector2f pos(frame.pos.x, frame.pos.y);
    array.append(sf::Vertex(pos - sf::Vector2f(-2, -2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(2, 2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(-2, 2), frame.color));
    array.append(sf::Vertex(pos - sf::Vector2f(2, -2), frame.color));

    sf::Vector2f dir(toVect(25 * abs(cos(frame.pitch)), frame.yaw));
    array.append(sf::Vertex(pos, frame.color));
    array.append(sf::Vertex(pos + dir, frame.color));
    array.append(sf::Vertex(pos + dir, frame.color));
    array.append(sf::Vertex(pos + dir + toVect(5, frame.yaw + 2.5f), frame.color));
    array.append(sf::Vertex(pos + dir, frame.color));
    array.append(sf::Vertex(pos + dir + toVect(5, frame.yaw - 2.5f), frame.color));
}
}

sf::VertexArray RoomRender::createLightsGismos() const
{
    std::shared_ptr<Patern> r(m_room.lock());
    if(!r)
        return sf::VertexArray();

    sf::VertexArray array(sf::Lines);

    float time = m_clock.getElapsedTime().asSeconds();

    for(unsigned int i(0) ; i < r->lightCount() ; i++)
    {
        const Light & l(r->light(i));

        for(unsigned int j(0) ; j < l.frameCount() ; j++)
        {
            unsigned int last = j == 0 ? l.frameCount()-1 : j-1;
            array.append(sf::Vertex(sf::Vector2f(l[j].pos.x, l[j].pos.y), sf::Color(l[j].color)));
            array.append(sf::Vertex(sf::Vector2f(l[last].pos.x, l[last].pos.y), sf::Color(l[last].color)));

            switch (l.type())
            {
            case LightType::POINT:
                drawPoint(array, l[j]);
            break;
            case LightType::SPOT:
                drawSpot(array, l[j]);

            break;
            case LightType::DIRECTIONNAL:
                drawDirectional(array, l[j]);
            break;
            default:
            break;
            }
        }

        switch (l.type())
        {
        case LightType::POINT:
            drawPoint(array, l.at(time));
        break;
        case LightType::SPOT:
            drawSpot(array, l.at(time));

        break;
        case LightType::DIRECTIONNAL:
            drawDirectional(array, l.at(time));
        break;
        default:
        break;
        }
    }
    return array;
}
