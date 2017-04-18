#ifndef TEXTUREVIEWERWIDGET_H
#define TEXTUREVIEWERWIDGET_H

#include <QWidget>
#include <QLabel>
#include "textureviewer.h"

class TextureViewerWidget : public QWidget
{
    Q_OBJECT

public:
    TextureViewerWidget(QWidget * parent = nullptr);
    virtual ~TextureViewerWidget() = default;

signals:
    void onConfigChanged();

public slots:
    void onIDSelected(unsigned int id);

private:
    TextureViewer *m_viewer;
    QLabel *m_selectIDLabel;
};

#endif // TEXTUREVIEWERWIDGET_H
