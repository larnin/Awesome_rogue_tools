#ifndef CONFIGS_H
#define CONFIGS_H

#include <QString>
#include <SFML/Graphics/Color.hpp>
#include "Gui/TileConfig/tileconfig.h"

class Configs
{
public:
    static void load(const QString & filename);
    static void save(const QString & filename);

    static const QString configsFilename;
    static const QString tileConfigFilename;

    static bool showGrid;
    static sf::Color gridColor;
    static bool showHitboxs;
    static TileConfig tiles;

private:
    Configs() = delete;
};

#endif // CONFIGS_H
