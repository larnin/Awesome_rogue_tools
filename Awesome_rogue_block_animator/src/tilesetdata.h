#ifndef TILESETDATA_H
#define TILESETDATA_H

#include "ressource.h"
#include "animation.h"
#include <QString>

class TilesetData
{
public:
    TilesetData() = default;
    void load(const QString & fileName);
    void save(const QString & fileName);

    static TilesetData instance;

    Texture texture;
    std::vector<Animation> animations;
    unsigned int tileSize = 16;
};

#endif // TILESETDATA_H
