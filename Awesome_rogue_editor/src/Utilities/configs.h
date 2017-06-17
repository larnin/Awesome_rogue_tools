#ifndef CONFIGS_H
#define CONFIGS_H

#include "Gui/TileConfig/tileconfig.h"
#include <SFML/Graphics/Color.hpp>
#include <QString>

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
    static bool showLights;
    static TileConfig tiles;

private:
    Configs() = delete;
};

#endif // CONFIGS_H
