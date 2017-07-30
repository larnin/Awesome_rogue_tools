#include "converter.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

Converter::Converter(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Image prenormal converter");

    m_filePos = new QLineEdit();
    m_filePos->setReadOnly(true);
    m_findFile = new QPushButton("...");
    m_findFile->setFixedWidth(50);

    m_tileSizeX = new QSpinBox();
    m_tileSizeX->setRange(0, 1000);
    m_tileSizeY = new QSpinBox();
    m_tileSizeY->setRange(0, 1000);
    m_borderSizeX = new QSpinBox();
    m_borderSizeX->setRange(0, 1000);
    m_borderSizeY = new QSpinBox();
    m_borderSizeY->setRange(0, 1000);

    m_addBorder = new QPushButton("Ajouter des bordures");
    m_removeBorder = new QPushButton("Supprimer des bordures");

    QHBoxLayout *fileLayout = new QHBoxLayout();
    fileLayout->addWidget(m_filePos, 1);
    fileLayout->addWidget(m_findFile);

    QHBoxLayout *tileSizeLayout = new QHBoxLayout();
    tileSizeLayout->addWidget(new QLabel("X : "));
    tileSizeLayout->addWidget(m_tileSizeX, 1);
    tileSizeLayout->addSpacing(10);
    tileSizeLayout->addWidget(new QLabel("Y : "));
    tileSizeLayout->addWidget(m_tileSizeY, 1);

    QHBoxLayout *tileBorderLayout = new QHBoxLayout();
    tileBorderLayout->addWidget(new QLabel("X : "));
    tileBorderLayout->addWidget(m_borderSizeX, 1);
    tileBorderLayout->addSpacing(10);
    tileBorderLayout->addWidget(new QLabel("Y : "));
    tileBorderLayout->addWidget(m_borderSizeY, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_addBorder);
    buttonsLayout->addWidget(m_removeBorder);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(new QLabel("Image :"));
    centralLayout->addLayout(fileLayout);
    centralLayout->addWidget(new QLabel("Taille de tile :"));
    centralLayout->addLayout(tileSizeLayout);
    centralLayout->addWidget(new QLabel("Taille de bordure :"));
    centralLayout->addLayout(tileBorderLayout);
    centralLayout->addSpacing(10);
    centralLayout->addLayout(buttonsLayout);

    setLayout(centralLayout);

    connect(m_findFile, SIGNAL(clicked(bool)), this, SLOT(onSelectImage()));
    connect(m_addBorder, SIGNAL(clicked(bool)), this, SLOT(onAddBorderClick()));
    connect(m_removeBorder, SIGNAL(clicked(bool)), this, SLOT(onRemoveBorderClick()));
}


void Converter::onSelectImage()
{
    QString file = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.xpm *.jpg)");
    if(file.isNull())
        return;
    m_filePos->setText(file);
}

void Converter::onAddBorderClick()
{
    if(!check())
        return;

    unsigned int tileX(m_tileSizeX->value());
    unsigned int tileY(m_tileSizeY->value());
    unsigned int singleBorderX(m_borderSizeX->value());
    unsigned int singleBorderY(m_borderSizeY->value());
    unsigned int borderX(tileX+2*singleBorderX);
    unsigned int borderY(tileY+2*singleBorderY);

    QImage img(m_filePos->text());
    unsigned int tilesX = img.size().width() / tileX;
    unsigned int tilesY = img.size().height() / tileY;
    if(tilesX == 0 || tilesY == 0)
    {
        QMessageBox::warning(this, "Taille ?", "Image trop petite !");
        return;
    }

    QImage outImg(tilesX * borderX, tilesY * borderY, QImage::Format_ARGB32);

    for(unsigned int x(0) ; x < tilesX ; x++)
        for(unsigned int y(0) ; y < tilesY ; y++)
        {
            for(unsigned int i(0) ; i < borderX ; i++)
                for(unsigned int j(0) ; j < borderY ; j++)
                {
                    int tI = i < singleBorderX ? 0 : i < singleBorderX + tileX ? i - singleBorderX : tileX - 1;
                    int tJ = j < singleBorderY ? 0 : j < singleBorderY + tileY ? j - singleBorderY : tileY - 1;
                    outImg.setPixel(x*borderX+i, y*borderY+j, img.pixel(x*tileX+tI, y*tileY+tJ));
                }
        }

    QString file = QFileDialog::getSaveFileName(this, "Save", "", "Images (*.png *.xpm *.jpg)");
    if(file.isNull())
        return;
    outImg.save(file);

    QMessageBox::information(this, "Success", "Image convertie !");
}

void Converter::onRemoveBorderClick()
{
    if(!check())
        return;

    unsigned int tileX(m_tileSizeX->value());
    unsigned int tileY(m_tileSizeY->value());
    unsigned int singleBorderX(m_borderSizeX->value());
    unsigned int singleBorderY(m_borderSizeY->value());
    unsigned int borderX(tileX+2*singleBorderX);
    unsigned int borderY(tileY+2*singleBorderY);

    QImage img(m_filePos->text());
    unsigned int tilesX = img.size().width() / borderX;
    unsigned int tilesY = img.size().height() / borderY;
    if(tilesX == 0 || tilesY == 0)
    {
        QMessageBox::warning(this, "Taille ?", "Image trop petite !");
        return;
    }

    QImage outImg(tilesX * tileX, tilesY * tileY, QImage::Format_ARGB32);

    for(unsigned int x(0) ; x < tilesX ; x++)
        for(unsigned int y(0) ; y < tilesY ; y++)
        {
            for(unsigned int i(0) ; i < tileX ; i++)
                for(unsigned int j(0) ; j < tileY ; j++)
                {
                    int tI = i + singleBorderX;
                    int tJ = j + singleBorderY;
                    outImg.setPixel(x*tileX+i, y*tileY+j, img.pixel(x*borderX+tI, y*borderY+tJ));
                }
        }

    QString file = QFileDialog::getSaveFileName(this, "Save", "", "Images (*.png *.xpm *.jpg)");
    if(file.isNull())
        return;
    outImg.save(file);

    QMessageBox::information(this, "Success", "Image convertie !");
}

bool Converter::check()
{
    if(m_filePos->text().isEmpty())
    {
        QMessageBox::warning(this, "Image ?", "Pas d'image selectionnée !");
        return false;
    }

    if(m_tileSizeX->value() < 1 || m_tileSizeY->value() < 1)
    {
        QMessageBox::warning(this, "Tile ?", "La taille d'une tile doit être positive !");
        return false;
    }

    if(m_borderSizeX->value() < 1 || m_borderSizeY->value() < 1)
    {
        QMessageBox::warning(this, "Bordure ?", "La taille de la bordure doit être positive !");
        return false;
    }

    return true;
}
