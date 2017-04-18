#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include <vector>
#include "block.h"
#include "hitbox.h"

enum TypeTile
{
    TILE_GROUND,
    TILE_WALL,
    TILE_OTHER
};

class BlockType
{
public:
    static bool isGround(unsigned int id);
    static bool isWall(unsigned int id);
    static BoxType boxOf(unsigned int id);
    static unsigned char baseBoxCaractOf(unsigned int id);
    static std::vector<unsigned int> getID(TypeTile type);
    static TypeTile typeOf(unsigned int id);
    static HitBox createBox(unsigned char boxCaracts);

    static sf::FloatRect texRect(unsigned int id, unsigned int blockSize, sf::Vector2u textureSize);

private:
    BlockType() = delete;
    ~BlockType() = delete;

};

#endif // BLOCKTYPE_H
