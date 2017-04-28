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
    void onItemSelectedChanged(int item);

signals:
    void defaultStateChanged(int);
    void newTransition(int);
    void selectedItemChanged(int);

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
    sf::VertexArray drawCurrentTransition();
    void makeTransition(unsigned int start, unsigned int end);

    Font m_font;
    int m_selectedItem;
    sf::Vector2f m_center;
    sf::Vector2f m_oldMousePos;

    bool m_moveItem;
    unsigned int m_itemIndex;

    bool m_makeTransition;
    unsigned int m_transitionStartItem;

};

#endif // ANIMATORCENTRALVIEW_H
