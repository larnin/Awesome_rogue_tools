#include "blocktype.h"
#include "Utilities/configs.h"
#include <algorithm>
#include <cassert>

const float PI(3.14159f);

bool BlockType::isGround(unsigned int id)
{
    if(id == 0)
        return true;
    if((id >= 384 && id <= 449) || id == 513)
        return true;
    return false;
}

bool BlockType::isWall(unsigned int id)
{
    if(id < 4 || id == 31)
        return true;
    if(id >= 64 && id <= 194)
        return true;
    if(id >= 256 && id <= 263)
        return true;
    if(id >= 288 && id <= 346)
        return true;
    if(id == 480)
        return true;
    if(id == 512)
        return true;
    return false;
}

BoxType BlockType::boxOf(unsigned int id)
{
    auto it(std::find_if(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [id](const auto & i){return id == i.id;}));
    if(it == Configs::tiles.walls.end())
        return EMPTY;
    return it->hitbox;

    /*if(typeOf(id) == TypeTile::TILE_GROUND || typeOf(id) == TypeTile::TILE_OTHER)
        return BoxType::EMPTY;
    if(id < 256 || id >= 480)
        return BoxType::FULL;

    if(id >= 256 && id < 288)
        return BoxType::TRIANGLE;
    if(id >= 288 && id < 292)
        return BoxType::QUARTER;
    if(id >= 292 && id < 316)
        return BoxType::HALF;
    if(id >= 316 && id < 320)
        return BoxType::THREE_QUARTER;
    if(id >= 320 && id <= 327)
        return BoxType::HALF_CENTRED;
    if(id == 328 || id == 329)
        return BoxType::EMPTY;
    if(id == 330 || id == 331)
        return BoxType::CROSS;
    if(id == 332 || id == 333)
        return BoxType::T_SHARP;
    if(id >= 334 && id <= 341)
        return BoxType::CORNER_CENTRED;
    if(id >= 342 && id <= 346)
        return BoxType::FULL;

    assert(false); //les autres types ne sont pas autorisé
    return BoxType::EMPTY;*/
}

unsigned char BlockType::baseBoxCaractOf(unsigned int id)
{
    auto it(std::find_if(Configs::tiles.walls.begin(), Configs::tiles.walls.end(), [id](const auto & i){return id == i.id;}));
    if(it == Configs::tiles.walls.end())
        return createBoxCaracts(ROT_0, false, false, EMPTY);
    return createBoxCaracts(it->hitboxRot, false, false, it->hitbox);
    /*
    Rotation rot(Rotation::ROT_0);

    //triangles
    if(id == 257 || id == 261)
        rot = Rotation::ROT_90;
    if(id == 258 || id == 262)
        rot = Rotation::ROT_270;
    if(id == 259 || id == 263)
        rot = Rotation::ROT_180;

    //quard
    if(id == 289)
        rot = Rotation::ROT_90;
    if(id == 290)
        rot = Rotation::ROT_270;
    if(id == 291)
        rot = Rotation::ROT_180;

    //demi
    if(id == 292 || id == 300 || id == 306)
        rot = Rotation::ROT_270;
    if(id == 293 || id == 301 || id == 307)
        rot = Rotation::ROT_90;
    if(id == 295 || id == 298 || id == 299 || id == 304 || id == 305 || id == 309 || id == 311 || id == 314 || id == 315)
        rot = Rotation::ROT_180;

    // 3 quards
    if(id == 317)
        rot = Rotation::ROT_90;
    if(id == 318)
        rot = Rotation::ROT_270;
    if(id == 319)
        rot = Rotation::ROT_180;

    //tubes T
    if(id == 333)
        rot = Rotation::ROT_180;

    //tubes en angle
    if(id == 335 || id == 339)
        rot = Rotation::ROT_90;
    if(id == 336 || id == 340)
        rot = Rotation::ROT_270;
    if(id == 337 || id == 341)
        rot = Rotation::ROT_180;

    return createBoxCaracts(rot, false, false, b);*/
}

std::vector<unsigned int> BlockType::getID(TypeTile type)
{
    if(type == TILE_GROUND)
        return Configs::tiles.groundIDs;
    if(type == TILE_TOP)
        return Configs::tiles.topIDs;
    std::vector<unsigned int> wallIds;
    for(const auto & item : Configs::tiles.walls)
        wallIds.push_back(item.id);
    return wallIds;

    /*std::vector<unsigned int> ids;
    const unsigned int max_ID(1024);
    for(unsigned int i(0) ; i < max_ID ; i++)
    {
        if(type == TypeTile::TILE_GROUND && isGround(i))
            ids.push_back(i);
        if(type == TypeTile::TILE_WALL && isWall(i))
            ids.push_back(i);
        if(type == TypeTile::TILE_OTHER && typeOf(i) == TypeTile::TILE_OTHER)
            ids.push_back(i);
    }
    return ids;*/
}

TypeTile  BlockType::typeOf(unsigned int id)
{
    if(isGround(id))
        return TypeTile::TILE_GROUND;
    if(isWall(id))
        return TypeTile::TILE_WALL;
    return TypeTile::TILE_OTHER;
}

HitBox BlockType::createBox(unsigned char boxCaracts)
{
    BoxType type(getBoxType(boxCaracts));
    HitBox box;
    switch (type)
    {
    case BoxType::FULL:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.5f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(-0.5f, -0.5f)));
        break;
    case BoxType::TRIANGLE:
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(-0.5f, 0.5f)));
        break;
    case BoxType::HALF:
        box.addLine(Line(sf::Vector2f(-0.5f, 0.0f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.0f), sf::Vector2f(-0.5f, 0.0f)));
        break;
    case BoxType::QUARTER:
        box.addLine(Line(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
        break;
    case BoxType::THREE_QUARTER:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(0.5f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.5f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(-0.5f, -0.5f)));
        /*box.addLine(Line(sf::Vector2f(-0.5f, -0.0f), sf::Vector2f(0.0f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.0f, -0.5f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(-0.5f, -0.0f)));*/
        break;
    case BoxType::HALF_CENTRED:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.25f), sf::Vector2f(-0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(-0.5f, -0.25f)));
        break;
    case BoxType::CROSS:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.25f), sf::Vector2f(-0.25f, -0.25f)));
        box.addLine(Line(sf::Vector2f(-0.25f, -0.25f), sf::Vector2f(-0.25f, -0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, -0.5f), sf::Vector2f(0.25f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, -0.5f), sf::Vector2f(0.25f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, -0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.25f), sf::Vector2f(0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.5f), sf::Vector2f(-0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.5f), sf::Vector2f(-0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.25f), sf::Vector2f(-0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.25f), sf::Vector2f(-0.5f, -0.25f)));
        break;
    case BoxType::T_SHARP:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.25f), sf::Vector2f(0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.5f), sf::Vector2f(-0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.5f), sf::Vector2f(-0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.25f), sf::Vector2f(-0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.25f), sf::Vector2f(-0.5f, -0.25f)));
        break;
    case BoxType::CORNER_CENTRED:
        box.addLine(Line(sf::Vector2f(-0.25f, -0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.25f), sf::Vector2f(0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.5f), sf::Vector2f(-0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.5f), sf::Vector2f(-0.25f, -0.25f)));
        break;
    default:
        break;
    }

    float rot(0.0f);
    switch(getRotation(boxCaracts))
    {
    case Rotation::ROT_90:
        rot = PI/2.0f;
        break;
    case Rotation::ROT_180:
        rot = PI;
        break;
    case Rotation::ROT_270:
        rot = 3*PI/2.0f;
        break;
    default:
        break;
    }
    return box.transform(rot, getXFlip(boxCaracts), getYFlip(boxCaracts));
}

sf::FloatRect BlockType::texRect(unsigned int id, unsigned int blockSize, sf::Vector2u textureSize)
{
    unsigned int blocksInWidth(textureSize.x/blockSize);
    sf::Vector2u blockPos(id%blocksInWidth, id/blocksInWidth);
    return sf::FloatRect(blockPos.x*blockSize, blockPos.y*blockSize, blockSize, blockSize);
}
