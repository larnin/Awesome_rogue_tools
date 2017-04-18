#ifndef BLOCKVIEW_H
#define BLOCKVIEW_H

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Gui/qsfmlcanvas.h"
#include "Utilities/ressource.h"
#include "Map/blocktype.h"

class BlockView : public QSFMLCanvas
{
    Q_OBJECT

public:
    BlockView(QWidget * parent = nullptr);
    virtual ~BlockView() = default;

signals:
    void selectBlock(unsigned int id);

public slots:
    void changeView(TypeTile type);
    void setBlock(unsigned int id);

protected:
    virtual void showEvent(QShowEvent* event);
    virtual void OnUpdate();
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);
    virtual void resizeEvent(QResizeEvent * event);

private:
    void drawTiles();
    void updateRect();
    sf::RectangleShape generactSelector() const;
    void checkSelected();

    sf::VertexArray m_array;
    bool m_selected;
    unsigned int m_selectedId;
    TypeTile m_type;

    float m_botomTiles;
};

#endif // BLOCKVIEW_H
