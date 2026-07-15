#pragma once

#include <algorithm>

#include "BaseSystem.hpp"
#include "../components/MovementComponent.hpp"
#include "../components/ColliderComponent.hpp"
#include "../../world/World.hpp"
#include "../../world/Tile.hpp"
#include "../../GLOBALS.hpp"

using namespace GLOBALS;

// Update() fits BaseSystem's (registry, delta) shape, so it rides SystemRegistry::
// UpdateAll alongside MovementSystem — that part's a normal system.
// DrawDebug() is deliberately separate and NOT part of Update(): it needs to run
// inside BeginDrawing/EndDrawing on the render cadence, same reasoning as
// RenderSystem being its own call site rather than folded into UpdateAll.
// It also stays on CollisionSystem rather than moving into RenderSystem, because
// collider boxes aren't a VisualComponent concern — RenderSystem doesn't know
// colliders exist, and shouldn't have to.
class CollisionSystem : public BaseSystem {
    public:
        CollisionSystem(World* world) : world(world) {}

        void Update(ComponentRegistry& registry, float delta) override {
            registry.ForEach<ColliderComponent>([&](uint32_t index, ColliderComponent& collider) {
                EntityID id = registry.EntityAt(index);
                MovementComponent* movement = registry.GetComponent<MovementComponent>(id);
                if (!movement) return;

                collider.grounded = false;

                constexpr int cell = TILESIZE * RENDERSCALE;

                float left, top, right, bottom;
                GetBounds(*movement, collider, left, top, right, bottom);

                int tileminx = FloorDiv((int)left, cell);
                int tilemaxx = FloorDiv((int)right, cell);
                int tileminy = FloorDiv((int)top, cell);
                int tilemaxy = FloorDiv((int)bottom, cell);

                for (int ty = tileminy; ty <= tilemaxy; ty++) for (int tx = tileminx; tx <= tilemaxx; tx++) {
                    Tile& tile = world->GetTile(tx, ty);
                    if (!IsSolid(tile.type)) continue;

                    ResolveAgainsTile(*movement, collider, tx, ty, cell);
                }
            });
        }

        void DrawDebug(ComponentRegistry& registry) {
            if (!DEBUGDRAWCOLLIDERS) return;

            registry.ForEach<ColliderComponent>([&](uint32_t index, ColliderComponent& collider) {
                EntityID id = registry.EntityAt(index);
                MovementComponent* movement = registry.GetComponent<MovementComponent>(id);
                if (!movement) return;

                float left, top, right, bottom;
                GetBounds(*movement, collider, left, top, right, bottom);

                Rectangle box{ left, top, right - left, bottom - top };
                DrawRectangleRec(box, Fade(GREEN, 0.25f));
                DrawRectangleLinesEx(box, 1.0f, GREEN);
            });
        }

    private:
        static void GetBounds(MovementComponent& movement, ColliderComponent& collider,
                               float& left, float& top, float& right, float& bottom) {
            left = movement.position.x - collider.width * 0.5f;
            right = movement.position.x + collider.width * 0.5f;
            bottom = movement.position.y;
            top = movement.position.y - collider.height;
        }

        void ResolveAgainsTile(MovementComponent& movement, ColliderComponent& collider, int tx, int ty, int cell) {
            float left, top, right, bottom;
            GetBounds(movement, collider, left, top, right, bottom);

            float tileleft = tx * cell, tiletop = ty * cell;
            float tileright = tileleft + cell, tilebottom = tiletop + cell;

            float overlapx = std::min(right, tileright) - std::max(left, tileleft);
            float overlapy = std::min(bottom, tilebottom) - std::max(top, tiletop);

            if (overlapx <= 0 || overlapy <= 0) return;

            if (overlapx < overlapy) {
                movement.position.x += (left < tileleft) ? -overlapx : overlapx;
                movement.velocity.x = 0.0f;
            } else {
                if (top < tiletop) {
                    movement.position.y -= overlapy;
                    movement.velocity.y = 0.0f;
                    collider.grounded = true;
                } else {
                    movement.position.y += overlapy;
                    movement.velocity.y = 0.0f;
                }
            }
        }

        World* world;
};