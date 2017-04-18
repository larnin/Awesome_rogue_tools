#ifndef NEWPATERNWIDGET_H
#define NEWPATERNWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include "Dock/paternsdock.h"

class NewPaternWidget : public QDialog
{
    Q_OBJECT
public:
    NewPaternWidget(QWidget *parent = 0);
    virtual ~NewPaternWidget() = default ;

signals:
    void valid(PaternInfo infos);

public slots:
    void validClicked();

private:
    QPushButton *m_valid;
    QPushButton *m_annule;
    QLineEdit *m_name;
    QSpinBox *m_sizeX;
    QSpinBox *m_sizeY;
};

#endif // NEWPATERNWIDGET_H
