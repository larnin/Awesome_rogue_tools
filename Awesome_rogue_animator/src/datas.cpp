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
        anim.insert("posX", s.pos.x);
        anim.insert("posy", s.pos.y);
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
        auto objState(value.toObject());
        emplace_back();
        back().animation = toAnimation(objState["animation"].toObject());
        for(const auto & transition : objState["transitions"].toArray())
            back().transitions.emplace_back(transition.toObject());
        back().pos.x = objState["posX"].toDouble();
        back().pos.y = objState["posY"].toDouble();
    }
}
