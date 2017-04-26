#include "animatorcentralview.h"
#include "datas.h"
#include "vect2convert.h"
#include "utilities.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <QMouseEvent>
#include <QMenu>

const sf::Vector2f boxSize(200, 50);
const float offset(5.0f);
const float PI(3.14159f);
const float gridSize(25);

AnimatorCentralView::AnimatorCentralView(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_font("res/calibri.ttf")
    , m_selectedItem(-1)
    , m_center(0, 0)
    , m_moveItem(false)
{
    setMinimumHeight(100);
    setMinimumWidth(100);
    setMouseTracking(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClick(QPoint)));
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
    updateView();

    RenderWindow::clear(sf::Color(51, 51, 51));

    RenderWindow::draw(drawGrid());

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
        if(i == Datas::instance().startIndex)
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
    QWidget::resizeEvent(event);
    updateView();
}

void AnimatorCentralView::mouseMoveEvent(QMouseEvent *event)
{
    sf::Vector2f offset(event->pos().x() - m_oldMousePos.x, event->pos().y() - m_oldMousePos.y);
    if(m_moveItem)
        moveItem(offset);
    else if(event->buttons().testFlag(Qt::LeftButton))
    {
        m_center -= offset;
        updateView();
    }

    m_oldMousePos = sf::Vector2f(event->pos().x(), event->pos().y());
}

void AnimatorCentralView::mousePressEvent(QMouseEvent *event)
{
    auto pos = RenderWindow::mapPixelToCoords(sf::Vector2i(event->pos().x(), event->pos().y()));

    if(event->button() == Qt::LeftButton)
    {
        for(unsigned int i(0) ; i < Datas::instance().size() ; i++)
        {
            const State & s(Datas::instance()[i]);
            sf::FloatRect rect(s.pos.x - boxSize.x/2, s.pos.y - boxSize.y/2, boxSize.x, boxSize.y);
            if(rect.contains(pos))
            {
                m_moveItem = true;
                m_itemIndex = i;
                return;
            }
        }
    }
}

void AnimatorCentralView::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
        m_moveItem = false;
}

void AnimatorCentralView::updateView()
{
    sf::View v(RenderWindow::getView());
    auto windowSize(RenderWindow::getSize());
    v.setSize(windowSize.x, windowSize.y);
    v.setCenter(m_center);
    RenderWindow::setView(v);
}

void AnimatorCentralView::moveItem(const sf::Vector2f & offset)
{
    if(!m_moveItem || m_itemIndex >= Datas::instance().size())
        return;

    Datas::instance()[m_itemIndex].pos += offset;
}


sf::VertexArray AnimatorCentralView::drawGrid() const
{
    const sf::Color linesColor(45, 45, 45);
    const sf::Color bigLinesColor(30, 30, 30);
    const sf::Color crossColor(240, 240, 240);
    const float crossSize(15);

    sf::View v(RenderWindow::getView());

    sf::VertexArray array(sf::Lines);

    sf::IntRect rect(sf::Vector2i((v.getCenter() - v.getSize()/2.0f)/gridSize), sf::Vector2i(v.getSize()/gridSize));
    for(int i(rect.left) ; i <= rect.left + rect.width ; i++)
    {
        const sf::Color & c(i%5 == 0 ? bigLinesColor : linesColor);
        array.append(sf::Vertex(sf::Vector2f(i*gridSize, v.getCenter().y - v.getSize().y/2), c));
        array.append(sf::Vertex(sf::Vector2f(i*gridSize, v.getCenter().y + v.getSize().y/2), c));
    }
    for(int i(rect.top) ; i <= rect.top + rect.height ; i++)
    {
        const sf::Color & c(i%5 == 0 ? bigLinesColor : linesColor);
        array.append(sf::Vertex(sf::Vector2f(v.getCenter().x - v.getSize().x/2, i*gridSize), c));
        array.append(sf::Vertex(sf::Vector2f(v.getCenter().x + v.getSize().x/2, i*gridSize), c));
    }

    sf::FloatRect bounds(v.getCenter() - v.getSize()/2.0f, v.getSize());

    if(!bounds.contains(sf::Vector2f(0, 0)))
    {
        if(bounds.left > 0 && bounds.top > 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10, bounds.top + 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10 + crossSize,  bounds.top + 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10,  bounds.top + 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10,  bounds.top + 10 + crossSize), crossColor));
        }
        else if(bounds.left + bounds.width < 0 && bounds.top > 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10, bounds.top + 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10 - crossSize,  bounds.top + 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10,  bounds.top + 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10,  bounds.top + 10 + crossSize), crossColor));
        }
        else if(bounds.left > 0 && bounds.top + bounds.height < 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10, bounds.top + bounds.height - 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10 + crossSize,  bounds.top + bounds.height - 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10,  bounds.top + bounds.height - 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10,  bounds.top + bounds.height - 10 - crossSize), crossColor));
        }
        else if(bounds.left + bounds.width < 0 && bounds.top + bounds.height < 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10, bounds.top + bounds.height - 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10 - crossSize,  bounds.top + bounds.height - 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10,  bounds.top + bounds.height - 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10,  bounds.top + bounds.height - 10 - crossSize), crossColor));
        }
        else if(bounds.top > 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width/2 - crossSize, bounds.top + 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width/2 + crossSize, bounds.top + 10), crossColor));
        }
        else if(bounds.top + bounds.height < 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width/2 - crossSize, bounds.top + bounds.height - 10), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width/2 + crossSize, bounds.top + bounds.height - 10), crossColor));
        }
        else if(bounds.left > 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10 , bounds.top + bounds.height/2 - crossSize), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + 10, bounds.top + bounds.height/2 + crossSize), crossColor));
        }
        else if(bounds.left + bounds.width < 0)
        {
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10 , bounds.top + bounds.height/2 - crossSize), crossColor));
            array.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width - 10, bounds.top + bounds.height/2 + crossSize), crossColor));
        }
    }
    else
    {
        array.append(sf::Vertex(sf::Vector2f(-crossSize, 0), crossColor));
        array.append(sf::Vertex(sf::Vector2f(crossSize, 0), crossColor));
        array.append(sf::Vertex(sf::Vector2f(0, -crossSize), crossColor));
        array.append(sf::Vertex(sf::Vector2f(0, crossSize), crossColor));
    }

    return array;
}

void AnimatorCentralView::onRightClick(QPoint p)
{
    auto pos = RenderWindow::mapPixelToCoords(sf::Vector2i(p.x(), p.y()));
    int index(-1);

    for(unsigned int i(0) ; i < Datas::instance().size() ; i++)
    {
        const State & s(Datas::instance()[i]);
        sf::FloatRect rect(s.pos.x - boxSize.x/2, s.pos.y - boxSize.y/2, boxSize.x, boxSize.y);
        if(rect.contains(pos))
        {
            index = i;
            break;
        }
    }
    if(index < 0)
        return;

    QAction *aTransition(nullptr), *aSetDefault(nullptr);
    QMenu menu;
    aTransition = menu.addAction("Ajouter une transition");
    aSetDefault = menu.addAction("Définir comme animation par defaut");

    QAction* action = menu.exec(p + globalWidgetPos(this));
    if(action == nullptr)
        return;

    if(action == aTransition)
    {
        //todo !
    }
    else if(action == aSetDefault)
    {
        Datas::instance().startIndex = index;
        emit defaultStateChanged(index);
    }

}

