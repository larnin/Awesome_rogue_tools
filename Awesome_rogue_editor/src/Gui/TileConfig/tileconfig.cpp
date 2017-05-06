#include "tileconfig.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

TileConfig::TileConfig()
    : tileSize(32)
{

}

TileConfig::TileConfig(const QString & filename)
    : TileConfig()
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
    tileSize = obj.value("size").toInt(32);
    texture.load(obj.value("texture").toString().toStdString());

    QJsonArray jGround(obj.value("ground").toArray());
    for(const auto & v : jGround)
        groundIDs.push_back(v.toInt(0));

    QJsonArray jTop(obj.value("top").toArray());
    for(const auto & v : jTop)
        topIDs.push_back(v.toInt(0));

    QJsonArray jWall(obj.value("wall").toArray());
    for(const auto & w : jWall)
    {
        QJsonObject o(w.toObject());
        TileWallInfo t(o.value("id").toInt(0), Rotation(o.value("rot").toInt(0)), BoxType(o.value("box").toInt(0)));
        walls.push_back(t);
    }
}

void TileConfig::save(const QString & filename) const
{
    QJsonArray jGround;
    for(unsigned int id : groundIDs)
        jGround.append(int(id));

    QJsonArray jTop;
    for(unsigned int id : topIDs)
        jTop.append(int(id));

    QJsonArray jWall;
    for(const TileWallInfo & tile : walls)
    {
        QJsonObject obj;
        obj.insert("id", int(tile.id));
        obj.insert("rot", tile.hitboxRot);
        obj.insert("box", tile.hitbox);
        jWall.append(obj);
    }

    QJsonObject obj;
    obj.insert("size", int(tileSize));
    obj.insert("texture", QString::fromStdString(texture.getFilename()));
    obj.insert("ground", jGround);
    obj.insert("wall", jWall);
    obj.insert("top", jTop);

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    file.close();
}
