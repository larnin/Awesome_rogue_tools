#ifndef TILECONFIG_H
#define TILECONFIG_H

#include <QString>
#include <vector>
#include "Map/block.h"
#include "Utilities/ressource.h"

struct TileWallInfo
{
    TileWallInfo(unsigned int _id, Rotation _rot, BoxType _hitbox)
        : id(_id), hitboxRot(_rot), hitbox(_hitbox) {}

    unsigned int id;
    Rotation hitboxRot;
    BoxType hitbox;
};

struct TileConfig
{
    TileConfig();
    TileConfig(const QString & filename);
    void save(const QString & filename) const;

    unsigned int tileSize;
    Texture texture;
    std::vector<unsigned int> groundIDs;
    std::vector<TileWallInfo> walls;
};

#endif // TILECONFIG_H
