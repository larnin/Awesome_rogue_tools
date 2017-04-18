#ifndef UTILITIES_H
#define UTILITIES_H

#include <QJsonDocument>
#include <QString>

void save(const QString & filename, const QJsonDocument & obj);
QJsonDocument load(const QString & filename);


#endif // UTILITIES_H
