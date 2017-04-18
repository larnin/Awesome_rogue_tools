#include "configs.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

const QString Configs::configsFilename("configs.json");
const QString Configs::tileConfigFilename("tiles.json");

bool Configs::showGrid(false);
sf::Color Configs::gridColor(sf::Color::White);
bool Configs::showHitboxs(false);
TileConfig Configs::tiles(tileConfigFilename);

void Configs::load(const QString & filename)
{
    QFile file(filename);
    if(!file.exists())
        return;
    if(!file.open(QIODevice::ReadOnly))
        return;

    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(file.readAll(), &error));
    file.close();
    if(error.error != QJsonParseError::NoError)
        return;
    if(!doc.isObject())
        return;

    QJsonObject obj(doc.object());
    showGrid = obj.value("grid").toBool(false);
    gridColor.r = abs(obj.value("gridr").toInt(255))%255;
    gridColor.g = abs(obj.value("gridg").toInt(255))%255;
    gridColor.b = abs(obj.value("gridb").toInt(255))%255;
    gridColor.a = abs(obj.value("grida").toInt(255))%255;
    showHitboxs = obj.value("box").toBool(false);
}

void Configs::save(const QString & filename)
{
    QJsonObject obj;
    obj.insert("grid", showGrid);
    obj.insert("gridr", gridColor.r);
    obj.insert("gridg", gridColor.g);
    obj.insert("gridb", gridColor.b);
    obj.insert("grida", gridColor.a);
    obj.insert("box", showHitboxs);

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    file.close();

    tiles.save(tileConfigFilename);
}
