#include "hitbox.h"
#include "Utilities/vect2convert.h"

Line Line::move(const sf::Vector2f & delta) const
{
    return Line(pos1+delta, pos2+delta);
}

bool Line::sameAs(const Line & l, float epsilon)
{
    if(epsilon < 0)
        return false;
    if(norm(l.pos1-pos1) < epsilon && norm(l.pos2-pos2) < epsilon)
        return true;
    if(norm(l.pos1-pos2) < epsilon && norm(l.pos2-pos1) < epsilon)
        return true;
    return false;
}

void HitBox::addLine(const Line & l)
{
    lines.push_back(l);
}

sf::FloatRect HitBox::globalRect() const
{
    const float epsilon(0.01f);
    sf::FloatRect rect;
    bool sizeCreated(false);

    for(auto & line : lines)
    {
        if(!sizeCreated)
        {
            sizeCreated = true;
            rect = sf::FloatRect(sf::Vector2f(std::min(line.pos1.x, line.pos2.x), std::min(line.pos1.y, line.pos2.y))
                            , sf::Vector2f(std::abs(line.pos1.x-line.pos2.x), std::abs(line.pos1.y-line.pos2.y)));
            continue;
        }
        float xMin(std::min(std::min(line.pos1.x, line.pos2.x), rect.left));
        float yMin(std::min(std::min(line.pos1.y, line.pos2.y), rect.top));
        float xMax(std::max(std::max(line.pos1.x, line.pos2.x), rect.left+rect.width));
        float yMax(std::max(std::max(line.pos1.y, line.pos2.y), rect.top+rect.height));
        rect = sf::FloatRect(xMin, yMin, xMax-xMin, yMax-yMin);
    }
    rect.left -= epsilon;
    rect.top -= epsilon;
    rect.width += 2*epsilon;
    rect.height += 2*epsilon;
    return rect;
}

HitBox HitBox::transform(float rotation, bool xFlip, bool yFlip) const
{
    HitBox out;

    for(const auto & line : lines)
    {
        Line l(rotate(line.pos1, rotation), rotate(line.pos2, rotation));
        if(xFlip)
        {
            l.pos1.x *= -1;
            l.pos2.x *= -1;
        }
        if(yFlip)
        {
            l.pos1.y *= -1;
            l.pos2.y *= -1;
        }
        out.addLine(l);
    }
    return out;
}

sf::VertexArray toRender(const HitBox & box, const sf::Color & color)
{
    sf::VertexArray array(sf::Lines, box.lines.size()*2);

    for(unsigned int i(0) ; i < box.lines.size() ; i++)
    {
        const Line & l(box.lines[i]);
        array[2*i] = sf::Vertex(l.pos1, color);
        array[2*i+1] = sf::Vertex(l.pos2, color);
    }
    return array;
}
