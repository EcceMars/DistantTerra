#pragma once

#include <algorithm>

#include "BaseComponent.hpp"
#include "MovementComponent.hpp"
#include "../../world/World.hpp"
#include "../../world/Tile.hpp"

class ColliderComponent : public BaseComponent {
    public:
        ColliderComponent(World* world, float width, float height) : world(world), width(width), height(height) {}

        bool grounded = false;

        void Init() override {
            movement = owner->GetComponent<MovementComponent>();
        }

        void Update(float delta) override {
            if (!movement) return;
            grounded = false;

            constexpr int cell = TILESIZE * RENDERSCALE;

            int tileminx = FloorDiv((int)movement->position.x, cell);
            int tilemaxx = FloorDiv((int)(movement->position.x + width), cell);
            int tileminy = FloorDiv((int)movement->position.y, cell);
            int tilemaxy = FloorDiv((int)(movement->position.y + height), cell);

            for (int ty = tileminy; ty <= tilemaxy; ty++) for (int tx = tileminx; tx <= tilemaxx; tx++) {
                Tile& tile = world->GetTile(tx, ty);
                if (!IsSolid(tile.type)) continue;

                ResolveAgainsTile(tx, ty, cell);
            }
        }

    private:
        void ResolveAgainsTile(int tx, int ty, int cell) {
            float left = movement->position.x, top = movement->position.y;
            float right = movement->position.x + width, bottom = movement->position.y + height;

            float tileleft = tx * cell, tiletop = ty * cell;
            float tileright = tileleft + cell, tilebottom = tiletop + cell;

            float overlapx = std::min(right, tileright) - std::max(left, tileleft);
            float overlapy = std::min(bottom, tilebottom) - std::max(top, tiletop);

            if (overlapx <= 0 || overlapy <= 0) return;

            if (overlapx < overlapy) {
                movement->position.x += (left < tileleft) ? -overlapx : overlapy;
                movement->velocity.y = 0.0f;
            } else {
                if (top < tiletop) {
                    movement->position.y -= overlapy;
                    movement->velocity.y = 0.0f;
                    grounded = true;
                } else {
                    movement->position.y += overlapy;
                    movement->velocity.y = 0.0f;
                }
            }
        }
    
        World* world;
        MovementComponent* movement = nullptr;
        float width, height;
};