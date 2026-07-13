#pragma once

#include <algorithm>

#include "raylib.h"
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


            // TASK!! bottom should always have a -4 pixels, so to have any sprite sit slightly inside the tile under it.
            // Would a set getter suffice?
            float left, top, right, bottom;
            GetBounds(left, top, right, bottom);

            int tileminx = FloorDiv((int)left, cell);
            int tilemaxx = FloorDiv((int)right, cell);
            int tileminy = FloorDiv((int)top, cell);
            int tilemaxy = FloorDiv((int)bottom, cell);

            for (int ty = tileminy; ty <= tilemaxy; ty++) for (int tx = tileminx; tx <= tilemaxx; tx++) {
                Tile& tile = world->GetTile(tx, ty);
                if (!IsSolid(tile.type)) continue;

                ResolveAgainsTile(tx, ty, cell);
            }
        }

        void Draw() override {
            if (!movement || !DEBUGDRAWCOLLIDERS) return;

            float left, top, right, bottom;
            GetBounds(left, top, right, bottom);

            Rectangle box{ left, top, right - left, bottom - top };
            DrawRectangleRec(box, Fade(GREEN, 0.25f));
            DrawRectangleLinesEx(box, 1.0f, GREEN);
        }

    private:
        // Anchored bottom-center: position.x sits at the horizontal midpoint,
        // position.y sits at the ground-contact line (the sprite's lowest pixel).
        void GetBounds(float& left, float& top, float& right, float& bottom) const {
            left = movement->position.x - width * 0.5f;
            right = movement->position.x + width * 0.5f;
            bottom = movement->position.y;
            top = movement->position.y - height;
        }

        void ResolveAgainsTile(int tx, int ty, int cell) {
            float left, top, right, bottom;
            GetBounds(left, top, right, bottom);

            float tileleft = tx * cell, tiletop = ty * cell;
            float tileright = tileleft + cell, tilebottom = tiletop + cell;

            float overlapx = std::min(right, tileright) - std::max(left, tileleft);
            float overlapy = std::min(bottom, tilebottom) - std::max(top, tiletop);

            if (overlapx <= 0 || overlapy <= 0) return;

            if (overlapx < overlapy) {
                movement->position.x += (left < tileleft) ? -overlapx : overlapx;
                movement->velocity.x = 0.0f;
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