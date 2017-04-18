#include "tile.h"

sf::FloatRect texRect(unsigned int id, unsigned int blockSize, sf::Vector2u textureSize)
{
    unsigned int blocksInWidth(textureSize.x/blockSize);
    sf::Vector2u blockPos(id%blocksInWidth, id/blocksInWidth);
    return sf::FloatRect(blockPos.x*blockSize, blockPos.y*blockSize, blockSize, blockSize);
}
