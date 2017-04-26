#ifndef UTILITIES_H
#define UTILITIES_H

#include <QJsonDocument>
#include <QString>
#include <QWidget>

void save(const QString & filename, const QJsonDocument & obj);
QJsonDocument load(const QString & filename);

QPoint globalWidgetPos(QWidget * widget);


#endif // UTILITIES_H
