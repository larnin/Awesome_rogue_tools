#ifndef TILECONFIGWIDGET_H
#define TILECONFIGWIDGET_H

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
#include "textureviewer.h"
#include "tileconfig.h"
#include "tileviewer.h"

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
    QGroupBox *m_isWall;
    QComboBox *m_wallBoxType;
    QRadioButton *m_rot0Box;
    QRadioButton *m_rot90Box;
    QRadioButton *m_rot180Box;
    QRadioButton *m_rot270Box;
    TileViewer *m_wallViewer;

    unsigned int m_currentTileID;
};

#endif // TILECONFIGWIDGET_H
