#ifndef UNDOINFOS_H
#define UNDOINFOS_H

#include "Map/block.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <stack>

struct BlockInfo
{
    BlockInfo(const sf::Vector2u & _pos, const Block & _b)
        : pos(_pos), block(_b) {}
    sf::Vector2u pos;
    Block block;
};

class UndoInfos
{
public:
    UndoInfos();
    ~UndoInfos() = default;

    void addPoint(const BlockInfo & b);
    void addPoint(const std::vector<BlockInfo> & b);
    std::vector<BlockInfo> takeLast();
    void clear();

private:
    std::stack<std::vector<BlockInfo>> m_blocks;
};

#endif // UNDOINFOS_H
