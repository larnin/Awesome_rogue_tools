#ifndef ANIMATIONTEXTUREVIEW_H
#define ANIMATIONTEXTUREVIEW_H

#include "ressource.h"
#include "animation.h"
#include "UI/qsfmlcanvas.h"
#include <SFML/System/Vector2.hpp>

class AnimationTextureView : public QSFMLCanvas
{
    Q_OBJECT

    enum class MovingPiece
    {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        TOP,
        LEFT,
        RIGHT,
        BOTTOM,
        CENTER,
        OFFSET,
        NONE
    };

public:
    AnimationTextureView(QWidget * parent = nullptr);
    virtual ~AnimationTextureView() = default;

signals:
    void changeFrame(unsigned int frameID, Frame f);

public slots:
    void onAnimationChange(Animation anim);
    void onTextureChange(Texture texture);
    void setBackgroundColor(sf::Color color);

protected:
    virtual void OnUpdate() override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;

private:
    void updateView();
    void renderCursors();
    void moveItem(float moveX, float moveY);
    static sf::Vector2f itemPos(const Frame & f, MovingPiece m);
    //void checkForCursors(int x, int y);

    Animation m_animation;
    Texture m_texture;
    Texture m_cursorsTexture;
    sf::Color m_backgroundColor;
    sf::Color m_squareColor;
    int m_zoomLevel;
    sf::Vector2f m_center;
    sf::Vector2f m_oldMousePos;

    bool m_moveItem;
    sf::Vector2f m_realItemPosition;
    Frame m_originalFrame;
    MovingPiece m_itemType;
    int m_frameIndex;
};

inline float zoomFromLevel(int level)
{
    return std::pow(std::sqrt(2), level);
}

#endif // ANIMATIONTEXTUREVIEW_H
