#include "datas.h"
#include "utilities.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Datas Datas::m_instance;

void Datas::saveDatas(const QString & filename)
{
    QJsonObject object;
    if(m_texture.isValid())
        object.insert("texture", QString::fromStdString(m_texture.name()));
    QJsonArray animations;
    for(const State & s : *this)
    {
        QJsonObject anim;
        anim.insert("animation", toJson(s.animation));
        QJsonArray transitions;
        for(const Transition & t : s.transitions)
            transitions.append(t.toJson());
        anim.insert("transitions", transitions);
        animations.append(anim);
    }
    object.insert("animations", animations);
    save(filename, QJsonDocument(object));
}

void Datas::loadDatas(const QString & filename)
{
    clear();
    QJsonDocument doc = load(filename);
    if(!doc.isObject())
        return;
    QJsonObject object(doc.object());
    m_texture.load(object["texture"].toString().toStdString());
    for(const auto & value : object["animations"].toArray())
    {

        emplace_back();
        back().animation = toAnimation(value.toObject()["animation"].toObject());
        for(const auto & transition : value.toObject()["transitions"].toArray())
            back().transitions.emplace_back(transition.toObject());
    }
}
