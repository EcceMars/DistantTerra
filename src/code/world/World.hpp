#pragma once
#include <unordered_map>
#include <cstddef>

#include "Chunk.hpp"

struct ChunkCoord {
    int x, y;
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && y == other.y;
    }
};

struct ChunkCoordHash {
    size_t operator()(const ChunkCoord& c) const {
        return std::hash<long long>()(
            (static_cast<long long>(c.x) << 32) ^ static_cast<unsigned int >(c.y)
        );
    }
};

inline int FloorDiv(int a, int b) {
    int q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) q--;
    return q;
}
inline int FloorMod(int a, int b) {
    int m = a % b;
    if (m != 0 && ((m < 0) != (b < 0))) m += b;
    return m;
}

class World {
    public:
        ChunkCoord WorldToChunkCoord(int worldX, int worldY) const {
            return { FloorDiv(worldX, Chunk::SIZE), FloorDiv(worldY, Chunk::SIZE) };
        }


        Tile& GetTile(int worldx, int worldy) {
            ChunkCoord cc = WorldToChunkCoord(worldx, worldy);
            Chunk& chunk = chunks[cc];
            int localx = FloorMod(worldx, Chunk::SIZE);
            int localy = FloorMod(worldy, Chunk::SIZE);
            return chunk.GetTile(localx, localy);
        }

        void ActivateChunksAround(ChunkCoord center, int radius) {
            for (int dy = -radius; dy <= radius; dy++) for (int dx = -radius; dx <= radius; dx++) {
                ChunkCoord cc{ center.x + dx, center.y + dy };
                chunks[cc].active = true;
            }
        }

        const std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash>& GetChunks() const {    return chunks;  }
    private:
        std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> chunks;
};