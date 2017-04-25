#ifndef ANIMATORCENTRALVIEW_H
#define ANIMATORCENTRALVIEW_H

#include "UI/qsfmlcanvas.h"
#include "ressource.h"

class AnimatorCentralView : public QSFMLCanvas
{
    Q_OBJECT
public:
    AnimatorCentralView(QWidget * parent  = nullptr);

public slots:

signals:

protected:
    virtual void OnUpdate() override;
    virtual void resizeEvent(QResizeEvent * event) override;

private:
    Font m_font;
    int m_selectedItem;
};

#endif // ANIMATORCENTRALVIEW_H
