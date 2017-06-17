#ifndef TILECONFIGWIDGET_H
#define TILECONFIGWIDGET_H

#include "textureviewer.h"
#include "tileconfig.h"
#include "tileviewer.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include <QComboBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QDoubleSpinBox>

class TileConfigWidget : public QWidget
{
    Q_OBJECT
public:
    TileConfigWidget(QWidget * parent = nullptr);

signals:
    void tileConfigChanged();

public slots:
    void onTileSelected(unsigned int id);
    void onTileSizeChanged(int value);
    void onValueChanged();

    void onSearchTexture();
    void onImportClicked();
    void onExportClicked();

    void onSearchNormal();
    void onResetNormal();
    void onMaterialValueChanged();

private:
    TileWallInfo createTile() const;

    QPushButton* m_importButton;
    QPushButton* m_exportButton;
    QPushButton* m_closeButton;

    QLineEdit* m_textureDirWidget;
    QPushButton* m_searchTextureButton;
    TextureViewer* m_textureViewerWidget;
    QSpinBox* m_tileSizeBox;

    QLabel *m_tileLabel;
    QCheckBox *m_isGround;
    QCheckBox *m_isTop;
    QGroupBox *m_isWall;
    QComboBox *m_wallBoxType;
    QRadioButton *m_rot0Box;
    QRadioButton *m_rot90Box;
    QRadioButton *m_rot180Box;
    QRadioButton *m_rot270Box;
    TileViewer *m_wallViewer;

    unsigned int m_currentTileID;

    QLineEdit *m_normalDirWidget;
    QPushButton *m_searchNormalButton;
    QPushButton *m_resetNormalButton;
    QDoubleSpinBox *m_ambiantWidget;
    QDoubleSpinBox *m_diffuseWidget;
    QDoubleSpinBox *m_specularWidget;
    QDoubleSpinBox *m_multiplierWidget;
};

#endif // TILECONFIGWIDGET_H
