#pragma once

enum class TileType {
    AIR = -1,
    GRASS = 0,
    SOIL = 1,
    STONE = 2,
    WATER = 3
};

enum class TileVariant {
    SURFACE = 0,
    BURIED = 1
};

inline int GetTileColumn(TileType type) {
    return static_cast<int>(type);
}
inline bool IsSolid(TileType type) {
    return type != TileType::AIR;
}

struct Tile {
    TileType type = TileType::AIR;
    TileVariant variant = TileVariant::SURFACE;
};