#include "animatorcentralview.h"
#include "datas.h"
#include "vect2convert.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

const sf::Vector2f boxSize(200, 50);
const float offset(5.0f);
const float PI(3.14159f);

AnimatorCentralView::AnimatorCentralView(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_font("res/calibri.ttf")
    , m_selectedItem(0)
{

}

void addOffsetAndArrowOnTransition(sf::VertexArray & array)
{
    sf::Vertex & v1 = array[array.getVertexCount() - 2];
    sf::Vertex & v2 = array[array.getVertexCount() - 1];

    float a(angle(v2.position-v1.position));
    v1.position += toVect(offset, a + PI/2);
    v2.position += toVect(offset, a + PI/2);

    sf::Vector2f center((v1.position + v2.position) / 2.0f);
    for(int i(0) ; i < 3 ; i++)
    {
        array.append(sf::Vertex(center  + toVect(1.5f*offset, a + i * PI / 3 * 2), sf::Color::White));
        array.append(sf::Vertex(center  + toVect(1.5f*offset, a + (i+1) * PI / 3 * 2), sf::Color::White));
    }
}

void AnimatorCentralView::OnUpdate()
{
    RenderWindow::clear(sf::Color(51, 51, 51));

    sf::VertexArray arrayTransitions(sf::Lines);

    for(const State & s : Datas::instance())
        for(const Transition & t : s.transitions)
        {
            if(t.targetAnimationID >= Datas::instance().size())
                continue;

            const State & s2 = Datas::instance()[t.targetAnimationID];
            arrayTransitions.append(sf::Vertex(s.pos, sf::Color::White));
            arrayTransitions.append(sf::Vertex(s2.pos, sf::Color::White));
            addOffsetAndArrowOnTransition(arrayTransitions);
        }
    RenderWindow::draw(arrayTransitions);

    for(unsigned int i(0) ; i < Datas::instance().size() ; i++)
    {
        const State & s(Datas::instance()[i]);

        sf::RectangleShape rect;
        rect.setPosition(s.pos - boxSize / 2.0f);
        rect.setFillColor(sf::Color(180, 180, 180));
        if(m_selectedItem == int(i))
        {
            rect.setOutlineThickness(2);
            rect.setOutlineColor(sf::Color::Green);
        }
        else
        {
            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color::Black);
        }
        if(i == 0)
            rect.setFillColor(sf::Color(255, 180, 130));

        rect.setSize(boxSize);
        RenderWindow::draw(rect);

        sf::Text text(std::to_string(i) + " : " + s.animation.name, *m_font, 16);
        text.setFillColor(sf::Color::Black);
        text.setPosition(s.pos - sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height)/2.0f - sf::Vector2f(text.getGlobalBounds().left, text.getGlobalBounds().top));
        text.setPosition(sf::Vector2f(sf::Vector2i(text.getPosition())));
        RenderWindow::draw(text);
    }
}

void AnimatorCentralView::resizeEvent(QResizeEvent * event)
{

}
