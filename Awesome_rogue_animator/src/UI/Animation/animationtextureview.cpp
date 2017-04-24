#include "animationtextureview.h"
#include "quadrender.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QApplication>

AnimationTextureView::AnimationTextureView(QWidget * parent)
    : QSFMLCanvas(20, parent)
    , m_cursorsTexture("res/cursors.png")
    , m_backgroundColor(sf::Color::White)
    , m_squareColor(sf::Color::Blue)
    , m_zoomLevel(0)
    , m_center(0, 0)
{
    setMinimumHeight(100);
    setMinimumWidth(100);
    setMouseTracking(true);
}


void AnimationTextureView::OnUpdate()
{
    RenderWindow::clear(m_backgroundColor);

    if(!m_texture.isValid())
        return;

    sf::Sprite s(*m_texture);
    RenderWindow::draw(s);

    renderCursors();
}

void AnimationTextureView::onAnimationChange(Animation anim)
{
    m_animation = anim;
}

void AnimationTextureView::onTextureChange(Texture texture)
{
    m_texture = texture;
    m_zoomLevel = 0;
    if(m_texture.isValid())
    {
        m_center = sf::Vector2f(m_texture->getSize().x/2, m_texture->getSize().y/2);
        updateView();
    }
}

void AnimationTextureView::setBackgroundColor(sf::Color color)
{
    m_backgroundColor = color;
}

void AnimationTextureView::updateView()
{
    sf::View v(RenderWindow::getView());
    auto windowSize(RenderWindow::getSize());
    float zoom(zoomFromLevel(m_zoomLevel));
    v.setSize(windowSize.x*zoom, windowSize.y*zoom);
    v.setCenter(m_center);
    RenderWindow::setView(v);
}

void AnimationTextureView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
        m_zoomLevel ++;
    if(event->angleDelta().y() < 0)
        m_zoomLevel --;
    if(m_zoomLevel < -10)
        m_zoomLevel = -10;
    if(m_zoomLevel > 10)
        m_zoomLevel = 10;
    updateView();
}

void AnimationTextureView::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    updateView();
}

void AnimationTextureView::mouseMoveEvent(QMouseEvent *event)
{
    float zoom(zoomFromLevel(m_zoomLevel));
    //checkForCursors(event->pos().x(), event->pos().y());
    if(m_moveItem)
    {
        moveItem((event->pos().x() - m_oldMousePos.x) * zoom, (event->pos().y() - m_oldMousePos.y) * zoom);
    }
    else if(event->buttons().testFlag(Qt::LeftButton))
    {
        m_center.x -= (event->pos().x() - m_oldMousePos.x) * zoom;
        m_center.y -= (event->pos().y() - m_oldMousePos.y) * zoom;
        updateView();
    }
    m_oldMousePos = sf::Vector2f(event->pos().x(), event->pos().y());
}

void AnimationTextureView::mousePressEvent(QMouseEvent *event)
{
    auto pos = RenderWindow::mapPixelToCoords(sf::Vector2i(event->pos().x(), event->pos().y()));

    for(unsigned int i(0) ; i < m_animation.frames.size() ; i++)
    {
        const Frame & f(m_animation.frames[i]);
        MovingPiece m(MovingPiece::NONE);
        if(pos.x >= f.offset.x + f.texRect.left - 3 && pos.x <= f.offset.x + f.texRect.left + 3
                && pos.y >= f.offset.y + f.texRect.top - 3 && pos.y <= f.offset.y + f.texRect.top + 3)
            m = MovingPiece::OFFSET;
        else if(pos.x >= f.texRect.left && pos.x <= f.texRect.left + 3
                && pos.y >= f.texRect.top && pos.y <= f.texRect.top + 3)
            m = MovingPiece::TOP_LEFT;
        else if(pos.x >= f.texRect.left + f.texRect.width - 3 && pos.x <= f.texRect.left + f.texRect.width
                && pos.y >= f.texRect.top && pos.y <= f.texRect.top + 3)
            m = MovingPiece::TOP_RIGHT;
        else if(pos.x >= f.texRect.left && pos.x <= f.texRect.left + 3
                && pos.y >= f.texRect.top + f.texRect.height - 3 && pos.y <= f.texRect.top + f.texRect.height)
            m = MovingPiece::BOTTOM_LEFT;
        else if(pos.x >= f.texRect.left + f.texRect.width - 3 && pos.x <= f.texRect.left + f.texRect.width
                && pos.y >= f.texRect.top + f.texRect.height - 3 && pos.y <= f.texRect.top + f.texRect.height)
            m = MovingPiece::BOTTOM_RIGHT;
        else if(pos.x >= f.texRect.left + f.texRect.width / 2 - 2 && pos.x <= f.texRect.left + f.texRect.width / 2 + 2
                && pos.y >= f.texRect.top && pos.y <= f.texRect.top + 2)
            m = MovingPiece::TOP;
        else if(pos.x >= f.texRect.left + f.texRect.width / 2 - 2 && pos.x <= f.texRect.left + f.texRect.width / 2 + 2
                && pos.y >= f.texRect.top + f.texRect.height - 2 && pos.y <= f.texRect.top + f.texRect.height)
            m = MovingPiece::BOTTOM;
        else if(pos.x >= f.texRect.left && pos.x <= f.texRect.left + 2
                && pos.y >= f.texRect.top + f.texRect.height / 2 - 2 && pos.y <= f.texRect.top + f.texRect.height / 2 + 2)
            m = MovingPiece::LEFT;
        else if(pos.x >= f.texRect.left + f.texRect.width - 2 && pos.x <= f.texRect.left + f.texRect.width
                && pos.y >= f.texRect.top + f.texRect.height / 2 - 2 && pos.y <= f.texRect.top + f.texRect.height / 2 + 2)
            m = MovingPiece::RIGHT;
        else if(pos.x >= f.texRect.left + f.texRect.width/2-2 && pos.x <= f.texRect.left + f.texRect.width/2+2
                && pos.y >= f.texRect.top + f.texRect.height/2-2 && pos.y <= f.texRect.top + f.texRect.height/2+2)
            m = MovingPiece::CENTER;

        if(m != MovingPiece::NONE)
        {
            m_moveItem = true;
            m_itemType = m;
            m_frameIndex = i;
            m_realItemPosition = itemPos(f, m);
            m_originalFrame = f;
            break;
        }
    }

    moveItem(0, 0);
}

void AnimationTextureView::mouseReleaseEvent(QMouseEvent *)
{
    m_moveItem = false;
}

void AnimationTextureView::moveItem(float moveX, float moveY)
{
    if(!m_moveItem)
        return;

    Frame & f(m_animation.frames[m_frameIndex]);
    m_realItemPosition += sf::Vector2f(moveX, moveY);
    sf::Vector2i posi(m_realItemPosition);
    sf::Vector2f pos(posi.x + ((int)f.texRect.width % 2 == 0 ? 0 : 0.5f), posi.y + ((int)f.texRect.height % 2 == 0 ? 0 : 0.5f));

    switch(m_itemType)
    {
    case MovingPiece::TOP_LEFT:
        f.texRect.left = posi.x;
        f.texRect.top = posi.y;
        f.texRect.width = m_originalFrame.texRect.width - (f.texRect.left - m_originalFrame.texRect.left);
        f.texRect.height = m_originalFrame.texRect.height - (f.texRect.top - m_originalFrame.texRect.top);
    break;
    case MovingPiece::TOP_RIGHT:
        f.texRect.width = posi.x - f.texRect.left;
        f.texRect.top = posi.y;
        f.texRect.height = m_originalFrame.texRect.height - (f.texRect.top - m_originalFrame.texRect.top);
    break;
    case MovingPiece::BOTTOM_LEFT:
        f.texRect.left = posi.x;
        f.texRect.width = m_originalFrame.texRect.width - (f.texRect.left - m_originalFrame.texRect.left);
        f.texRect.height = posi.y - f.texRect.top;
    break;
    case MovingPiece::BOTTOM_RIGHT:
        f.texRect.width = posi.x - f.texRect.left;
        f.texRect.height = posi.y - f.texRect.top;
    break;
    case MovingPiece::TOP:
        f.texRect.top = posi.y;
        f.texRect.height = m_originalFrame.texRect.height - (f.texRect.top - m_originalFrame.texRect.top);
    break;
    case MovingPiece::LEFT:
        f.texRect.left = posi.x;
        f.texRect.width = m_originalFrame.texRect.width - (f.texRect.left - m_originalFrame.texRect.left);
    break;
    case MovingPiece::RIGHT:
        f.texRect.width = posi.x - f.texRect.left;
    break;
    case MovingPiece::BOTTOM:
        f.texRect.height = posi.y - f.texRect.top;
    break;
    case MovingPiece::CENTER:
        f.texRect.left = pos.x - f.texRect.width/2;
        f.texRect.top = pos.y - f.texRect.height/2;
    break;
    case MovingPiece::OFFSET:
        f.offset.x = pos.x - f.texRect.left;
        f.offset.y = pos.y - f.texRect.top;
    break;
    default : //MovingPiece::NONE
    break;
    }

    if(f.texRect.width < 1)
        f.texRect.width = 1;
    if(f.texRect.height < 1)
        f.texRect.height = 1;

    emit changeFrame(m_frameIndex, f);
}

void AnimationTextureView::renderCursors()
{
    if(m_animation.frames.empty())
        return;

    sf::VertexArray array(sf::Lines, m_animation.frames.size()*8);
    sf::VertexArray icones(sf::Quads, m_animation.frames.size()*40); //10 quads
    for(unsigned int i(0) ; i < m_animation.frames.size() ; i++)
    {
        for(unsigned int j(0) ; j < 8 ; j++)
            array[8*i+j].color = m_squareColor;
        const Frame & f(m_animation.frames[i]);
        array[8*i].position = sf::Vector2f(f.texRect.left, f.texRect.top);
        array[8*i+1].position = sf::Vector2f(f.texRect.left + f.texRect.width, f.texRect.top);
        array[8*i+2].position = sf::Vector2f(f.texRect.left + f.texRect.width, f.texRect.top);
        array[8*i+3].position = sf::Vector2f(f.texRect.left + f.texRect.width, f.texRect.top + f.texRect.height);
        array[8*i+4].position = sf::Vector2f(f.texRect.left + f.texRect.width, f.texRect.top + f.texRect.height);
        array[8*i+5].position = sf::Vector2f(f.texRect.left, f.texRect.top + f.texRect.height);
        array[8*i+6].position = sf::Vector2f(f.texRect.left, f.texRect.top + f.texRect.height);
        array[8*i+7].position = sf::Vector2f(f.texRect.left, f.texRect.top);

        drawQuad(&icones[40*i], sf::FloatRect(f.texRect.left, f.texRect.top, 4, 4), sf::FloatRect(0, 0, 4, 4));
        drawQuad(&icones[40*i+4], sf::FloatRect(f.texRect.left+f.texRect.width-4, f.texRect.top, 4, 4), sf::FloatRect(0, 0, 4, 4), false, false, Rotation::ROT_270);
        drawQuad(&icones[40*i+8], sf::FloatRect(f.texRect.left+f.texRect.width-4, f.texRect.top+f.texRect.height-4, 4, 4), sf::FloatRect(0, 0, 4, 4), false, false, Rotation::ROT_180);
        drawQuad(&icones[40*i+12], sf::FloatRect(f.texRect.left, f.texRect.top+f.texRect.height-4, 4, 4), sf::FloatRect(0, 0, 4, 4), false, false, Rotation::ROT_90);

        drawQuad(&icones[40*i+16], sf::FloatRect(f.texRect.left+f.texRect.width/2-2, f.texRect.top, 5, 3), sf::FloatRect(5, 0, 5, 3));
        drawQuad(&icones[40*i+20], sf::FloatRect(f.texRect.left, f.texRect.top+f.texRect.height/2-2, 3, 5), sf::FloatRect(5, 0, 5, 3), false, false, Rotation::ROT_90);
        drawQuad(&icones[40*i+24], sf::FloatRect(f.texRect.left+f.texRect.width/2-2, f.texRect.top+f.texRect.height-3, 5, 3), sf::FloatRect(5, 0, 5, 3), false, false, Rotation::ROT_180);
        drawQuad(&icones[40*i+28], sf::FloatRect(f.texRect.left+f.texRect.width-3, f.texRect.top+f.texRect.height/2-2, 3, 5), sf::FloatRect(5, 0, 5, 3), false, false, Rotation::ROT_270);

        drawQuad(&icones[40*i+32], sf::FloatRect(f.texRect.left+f.texRect.width/2-2, f.texRect.top+f.texRect.height/2-2, 5, 5), sf::FloatRect(5, 4, 5, 5));
        drawQuad(&icones[40*i+36], sf::FloatRect(f.texRect.left+f.offset.x-3, f.texRect.top+f.offset.y-3, 7, 7), sf::FloatRect(11, 0, 7, 7));
    }

    RenderWindow::draw(array);
    RenderWindow::draw(icones, sf::RenderStates(m_cursorsTexture()));
}

sf::Vector2f AnimationTextureView::itemPos(const Frame & f, MovingPiece m)
{
    switch(m)
    {
    case MovingPiece::TOP_LEFT:
        return sf::Vector2f(f.texRect.left, f.texRect.top);
    case MovingPiece::TOP_RIGHT:
        return sf::Vector2f(f.texRect.left + f.texRect.width, f.texRect.top);
    case MovingPiece::BOTTOM_LEFT:
        return sf::Vector2f(f.texRect.left, f.texRect.top + f.texRect.height);
    case MovingPiece::BOTTOM_RIGHT:
        return sf::Vector2f(f.texRect.left + f.texRect.width, f.texRect.top + f.texRect.height);
    case MovingPiece::TOP:
        return sf::Vector2f(f.texRect.left + f.texRect.width / 2, f.texRect.top);
    case MovingPiece::LEFT:
        return sf::Vector2f(f.texRect.left, f.texRect.top + f.texRect.height / 2);
    case MovingPiece::RIGHT:
        return sf::Vector2f(f.texRect.left + f.texRect.width, f.texRect.top + f.texRect.height / 2);
    case MovingPiece::BOTTOM:
        return sf::Vector2f(f.texRect.left + f.texRect.width / 2, f.texRect.top + f.texRect.height);
    case MovingPiece::CENTER:
        return sf::Vector2f(f.texRect.left + f.texRect.width / 2, f.texRect.top + f.texRect.height / 2);
    case MovingPiece::OFFSET:
        return sf::Vector2f(f.texRect.left + f.offset.x, f.texRect.top + f.offset.y);
    default : //MovingPiece::NONE
        return sf::Vector2f();
    }
}

/*
void AnimationTextureView::checkForCursors(int x, int y)
{
    auto pos = RenderWindow::mapPixelToCoords(sf::Vector2i(x, y));

    for(const Frame & f : m_animation.frames)
    {
        if(pos.x >= f.texRect.left && pos.x <= f.texRect.left+4 && pos.y >= f.texRect.top && pos.y <= f.texRect.top + 4)
            QApplication::changeOverrideCursor(Qt::SizeFDiagCursor);
    }
    QApplication::changeOverrideCursor(Qt::ArrowCursor);
}*/
