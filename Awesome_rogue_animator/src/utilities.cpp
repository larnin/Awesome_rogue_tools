#include "utilities.h"
#include <QFile>

void save(const QString & filename, const QJsonDocument & obj)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(obj.toJson(QJsonDocument::Compact));
    file.close();
}

QJsonDocument load(const QString & filename)
{
    QFile file(filename);
    if(!file.exists())
        return QJsonDocument();
    if(!file.open(QIODevice::ReadOnly))
        return QJsonDocument();

    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(file.readAll(), &error));
    file.close();
    if(error.error != QJsonParseError::NoError)
        return QJsonDocument();
    return doc;
}
