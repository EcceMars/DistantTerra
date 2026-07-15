#pragma once

#include "BaseSystem.hpp"
#include "../components/WorldComponent.hpp"
#include "../components/MovementComponent.hpp"
#include "../../world/World.hpp"
#include "../../GLOBALS.hpp"

using namespace GLOBALS;

class WorldSystem : public BaseSystem {
    public:
        WorldSystem(World* world) : world(world) {}

        void Update(ComponentRegistry& registry, float delta) override {
            registry.ForEach<WorldComponent>([&](uint32_t index, WorldComponent& area) {
                EntityID id = registry.EntityAt(index);
                MovementComponent* movement = registry.GetComponent<MovementComponent>(id);
                Vector2 position = movement ? movement->position : area.anchor;

                int tilex = (int)(position.x / (TILESIZE * RENDERSCALE));
                int tiley = (int)(position.y / (TILESIZE * RENDERSCALE));
                ChunkCoord center = world->WorldToChunkCoord(tilex, tiley);
                world->ActivateChunksAround(center, area.radius);
            });
        }

    private:
        World* world;
};