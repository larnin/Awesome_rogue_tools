#include "textureviewerwidget.h"
#include <QVBoxLayout>

TextureViewerWidget::TextureViewerWidget(QWidget * parent)
    : QWidget(parent)
{
    setWindowFlags ( Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint);
    setWindowTitle("Texture viewer");

    m_viewer = new TextureViewer();
    m_viewer->setMinimumHeight(100);
    m_selectIDLabel = new QLabel("Block séléctionné : 0");
    QFont font(m_selectIDLabel->font());
    font.setPointSize(20);
    m_selectIDLabel->setFont(font);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_viewer, 1);
    layout->addWidget(m_selectIDLabel);

    setLayout(layout);

    connect(m_viewer, SIGNAL(TileSelected(uint)), this, SLOT(onIDSelected(uint)));
    connect(this, SIGNAL(onConfigChanged()), m_viewer, SLOT(onConfigChanged()));

    resize(500, 400);
}

void TextureViewerWidget::onIDSelected(unsigned int id)
{
    m_selectIDLabel->setText("Block séléctionné : " + QString::number(id));
}
