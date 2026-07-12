#pragma once

#include "BaseComponent.hpp"
#include "MovementComponent.hpp"
#include "../../world/World.hpp"

class WorldComponent : public BaseComponent {
    public:
        WorldComponent(World* world, int radius = 2) : world(world), radius(radius) {}

        void Init() override {
            movement = owner->GetComponent<MovementComponent>();
        }

        void Update(float delta) override {
            if (!movement) return;

            int tilex = (int)(movement->position.x / (TILESIZE * RENDERSCALE));
            int tiley = (int)(movement->position.y / (TILESIZE * RENDERSCALE));
            ChunkCoord center = world->WorldToChunkCoord(tilex, tiley);
            world->ActivateChunksAround(center, radius);
        }

    private:
        World* world;
        MovementComponent* movement = nullptr;
        int radius;

};