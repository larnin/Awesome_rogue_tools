#include "tilesetdata.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

TilesetData TilesetData::instance;

void TilesetData::load(const QString & fileName)
{
    animations.clear();

    QFile file(fileName);
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
    QJsonObject docObj(doc.object());

    texture.load(docObj.value("texture").toString().toStdString());

    QJsonArray array(docObj.value("blocks").toArray());
    for(const auto & jBlock : array)
    {
        QJsonObject oBlock(jBlock.toObject());
        Animation a(oBlock.value("id").toInt());
        for(const auto & jAnim : oBlock.value("anims").toArray())
        {
            QJsonObject oAnim(jAnim.toObject());
            SingleAnimation sA(oAnim.value("weight").toDouble());
            for(const auto & jFrame : oAnim.value("frames").toArray())
            {
                QJsonObject oFrame(jFrame.toObject());
                Frame f(oFrame.value("mintime").toDouble()
                        , oFrame.value("maxtime").toDouble()
                        , oFrame.value("id").toInt()
                        , Rotation(oFrame.value("rot").toInt())
                        , oFrame.value("flipx").toBool()
                        , oFrame.value("flipy").toBool());
                sA.frames.push_back(f);
            }
            a.anims.push_back(sA);
        }
        animations.push_back(a);
    }
}

void TilesetData::save(const QString & fileName)
{
    QJsonObject obj;
    obj.insert("texture", QString::fromStdString(texture.getFilename()));

    QJsonArray jBlocks;
    for(const Animation & a : animations)
    {
        QJsonObject jBlock;
        jBlock.insert("id", int(a.id));
        QJsonArray jAnims;
        for(const SingleAnimation & sA : a.anims)
        {
            QJsonObject jAnim;
            jAnim.insert("weight", sA.weight);
            QJsonArray jFrames;
            for(const Frame & f : sA.frames)
            {
                QJsonObject jFrame;
                jFrame.insert("mintime", f.minTime);
                jFrame.insert("maxtime", f.maxTime);
                jFrame.insert("id", int(f.id));
                jFrame.insert("flipx", f.flipX);
                jFrame.insert("flipy", f.flipY);
                jFrame.insert("rot", int(f.rot));
                jFrames.append(jFrame);
            }
            jAnim.insert("frames", jFrames);
            jAnims.append(jAnim);
        }
        jBlock.insert("anims", jAnims);
        jBlocks.append(jBlock);
    }
    obj.insert("blocks", jBlocks);

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    file.close();
}
