#ifndef ANIMATORCONFIG_H
#define ANIMATORCONFIG_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include "textureviewerwidget.h"
#include "blockanimator.h"

class AnimatorConfig : public QWidget
{
    Q_OBJECT
public:
    AnimatorConfig(QWidget * parent = nullptr);
    virtual ~AnimatorConfig() = default;

signals:
    void listChanged();

public slots:
    void onRightClickBlocks(const QPoint& point);
    void onRightClickAnims(const QPoint& point);
    void onRightClickFrames(const QPoint& point);

    void reloadFromBlocks();
    void reloadFromAnims();
    void reloadFromFrames();
    void updateFrameData();

    void frameValueChanged();
    void tileSizeChanged();

    void loadClicked();
    void saveClicked();
    void clearClicked();
    void searchClicked();

protected:
    virtual void showEvent(QShowEvent *e);
    virtual void closeEvent(QCloseEvent *e);

private:
    void blockSignalsFromBlocks(bool value);
    void blockSignalsFromAnims(bool value);
    void blockSignalsFromFrames(bool value);
    void blockSignalsFromFrame(bool value);

    QSpinBox *m_tileSizeWidget;

    QPushButton *m_loadButton;
    QPushButton *m_saveButton;
    QPushButton *m_clearButton;
    QPushButton *m_searchTexture;
    QLineEdit *m_textureNameWidget;

    QListWidget *m_blocksWidget;
    QListWidget *m_animsWidget;
    QListWidget *m_framesWidget;

    QSpinBox *m_idFrame;
    QCheckBox *m_flipXFrame;
    QCheckBox *m_flipYFrame;
    QRadioButton *m_rot0;
    QRadioButton *m_rot90;
    QRadioButton *m_rot180;
    QRadioButton *m_rot270;
    QDoubleSpinBox *m_minTimeFrame;
    QDoubleSpinBox *m_maxTimeFrame;

    QGroupBox *m_frameBox;

    TextureViewerWidget m_viewer;
    BlockAnimator m_animator;
};

#endif // ANIMATORCONFIG_H
