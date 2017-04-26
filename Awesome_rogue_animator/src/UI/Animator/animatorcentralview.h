#ifndef ANIMATORCENTRALVIEW_H
#define ANIMATORCENTRALVIEW_H

#include "UI/qsfmlcanvas.h"
#include "ressource.h"
#include <SFML/Graphics/VertexArray.hpp>

class AnimatorCentralView : public QSFMLCanvas
{
    Q_OBJECT
public:
    AnimatorCentralView(QWidget * parent  = nullptr);

public slots:
    void onRightClick(QPoint p);

signals:
    void defaultStateChanged(int);

protected:
    virtual void OnUpdate() override;
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void updateView();
    void moveItem(const sf::Vector2f & offset);
    sf::VertexArray drawGrid() const;

    Font m_font;
    int m_selectedItem;
    sf::Vector2f m_center;
    sf::Vector2f m_oldMousePos;

    bool m_moveItem;
    unsigned int m_itemIndex;
};

#endif // ANIMATORCENTRALVIEW_H
