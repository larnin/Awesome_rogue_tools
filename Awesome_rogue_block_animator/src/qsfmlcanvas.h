#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    explicit QSFMLCanvas(unsigned int FrameTime = 20, QWidget *parent = nullptr);
    virtual ~QSFMLCanvas() = default;

protected:
    virtual void showEvent(QShowEvent*);
    virtual QPaintEngine* paintEngine() const;
    virtual void paintEvent(QPaintEvent*);
    virtual void OnUpdate() = 0;
    virtual void resizeEvent(QResizeEvent * event);

private:
    QTimer m_timer;
    bool m_initialized;
};

#endif // QSMLCANVAS_H
