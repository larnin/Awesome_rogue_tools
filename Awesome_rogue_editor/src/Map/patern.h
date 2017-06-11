#ifndef PATERN_H
#define PATERN_H

#include "Utilities/matrix.h"
#include "Map/block.h"
#include "Map/light.h"
#include <vector>
#include <array>
#include <memory>

class Patern
{
    friend class Room;

public:
    enum RoomType
    {
        NORMAL_ROOM,
        BOSS1_ROOM,
        BOSS2_ROOM,
        BOSS3_ROOM,
        BOSS4_ROOM,
        FINAL_ROOM,
        START_ROOM
    };

    Patern(sf::Vector2u size);
    ~Patern() = default;
    sf::Vector2u getSize() const;
    Block & operator()(const sf::Vector2u & pos);
    const Block & operator()(const sf::Vector2u & pos) const;
    Patern transform(Rotation rot, bool flipX, bool flipY);

    inline unsigned int lightCount() const {return m_lights.size();}
    inline Light & light(int index) {return m_lights[index];}
    inline const Light & light(int index) const { return m_lights[index]; }
    inline void addLight(const Light & l) { m_lights.push_back(l); }
    inline void delLightAt(unsigned int index) { m_lights.erase(std::next(m_lights.begin(), index)); }

    static std::vector<std::shared_ptr<Patern>> load(const std::string & fileName);
    static void save(const std::string & fileName, const std::vector<std::shared_ptr<Patern>> & paterns);

    RoomType type;
    std::string name;
    unsigned int rarity;

private:
    Patern(sf::Vector2u size, Block def);
    void generate();

    static sf::Vector2i transformPos(const sf::Vector2u & pos, const std::array<int,4> & transform, const sf::Vector2i & newSize);
    static std::array<int,4> multiplyMatrice(const std::array<int,4> & left, const std::array<int,4> & right);

    Matrix<Block> m_blocks;

    std::vector<Light> m_lights;
};

#endif // PATERN_H
