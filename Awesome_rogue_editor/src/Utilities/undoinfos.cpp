#include "undoinfos.h"

UndoInfos::UndoInfos()
{

}

void UndoInfos::addPoint(const BlockInfo & b)
{
    m_blocks.push(std::vector<BlockInfo>(1, b));
}

void UndoInfos::addPoint(const std::vector<BlockInfo> & b)
{
    m_blocks.push(b);
}

std::vector<BlockInfo> UndoInfos::takeLast()
{
    if(m_blocks.size() == 0)
        return std::vector<BlockInfo>();
    auto b(m_blocks.top());
    m_blocks.pop();
    return b;
}

void UndoInfos::clear()
{
    m_blocks = std::stack<std::vector<BlockInfo>>();
}
