#ifndef TILECONFIG_H
#define TILECONFIG_H

#include "Map/block.h"
#include "Utilities/ressource.h"
#include <QString>
#include <vector>

Texture defaultNormalMap();

struct Material
{
    Material();
    Material(Texture _normal);

    Texture normal;

    float ambiantCoeficient;
    float diffuseCoefficient;
    float specularCoefficient;
    float specularMultiplier;
};

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
    std::vector<unsigned int> topIDs;
    std::vector<TileWallInfo> walls;
    Material material;

    sf::Color ambiantColor;
};

#endif // TILECONFIG_H
