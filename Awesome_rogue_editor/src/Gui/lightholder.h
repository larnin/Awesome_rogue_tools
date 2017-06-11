#ifndef LIGHTHOLDER_H
#define LIGHTHOLDER_H

#include "Map/light.h"
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <vector>

class LightHolder : public QWidget
{
    Q_OBJECT
public:
    LightHolder(Light & light, QWidget * parent = nullptr);

public slots:
    void onLightTypeChange(int index);
    void onAddClicked();
    void onDelClicked();

private:
    void initializeFrames();
    void add(LightType type, unsigned int index);
    void delLast();

    QComboBox * m_lightType;
    QFrame *m_frame;
    std::vector<QWidget*> m_widgets;
    QPushButton* m_addButton;
    QPushButton* m_delButton;

    Light & m_light;
};

#endif // LIGHTHOLDER_H
