#pragma once
#include <array>
#include "Tile.hpp"

class Chunk {
    public:
        static constexpr int SIZE = 16;

        Tile& GetTile(int localx, int localy) {
            return tiles[localy * SIZE + localx];
        }

        const Tile& GetTile(int localx, int localy) const {
            return tiles[localy * SIZE + localx];
        }

        bool active = false;

    private:
        std::array<Tile, SIZE * SIZE> tiles{};
};