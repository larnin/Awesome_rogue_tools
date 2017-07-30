#ifndef CONVERTER_H
#define CONVERTER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

class Converter : public QWidget
{
    Q_OBJECT

public:
    Converter(QWidget *parent = nullptr);
    ~Converter() = default;

public slots:
    void onSelectImage();
    void onAddBorderClick();
    void onRemoveBorderClick();

private:
    bool check();

    QLineEdit *m_filePos;
    QPushButton *m_findFile;
    QSpinBox *m_tileSizeX;
    QSpinBox *m_tileSizeY;
    QSpinBox *m_borderSizeX;
    QSpinBox *m_borderSizeY;
    QPushButton *m_addBorder;
    QPushButton *m_removeBorder;
};

#endif // CONVERTER_H
