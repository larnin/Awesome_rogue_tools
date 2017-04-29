#include <QResizeEvent>
#include "qsfmlcanvas.h"

QSFMLCanvas::QSFMLCanvas(unsigned int FrameTime, QWidget *parent)
    : QWidget(parent)
    , m_initialized (false)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);
    m_timer.setInterval(FrameTime);
}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!m_initialized)
    {
        RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()), sf::ContextSettings(0, 0, 4));

        connect(&m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
        m_timer.start();
        m_initialized = true;
    }
}

void QSFMLCanvas::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    sf::View v(RenderWindow::getView());
    v.setSize(event->size().width(), event->size().height());
    RenderWindow::setView(v);
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return nullptr;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    sf::Event event;
    while(RenderWindow::pollEvent(event));

    OnUpdate();

    RenderWindow::display();
}
