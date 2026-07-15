#pragma once

#include <cmath>

#include "raylib.h"
#include "BaseSystem.hpp"
#include "../components/HealthComponent.hpp"
#include "../components/MovementComponent.hpp"
#include "../../GLOBALS.hpp"

class MovementSystem : public BaseSystem {
    public:
        void Update(ComponentRegistry& registry, float delta) override {
            registry.ForEach<MovementComponent>([&](uint32_t index, MovementComponent& movement) {
                movement.velocity.y += GRAVITYBASE * delta;

                if (!movement.waypoints.empty()) {
                    SteerTowardWaypoint(registry, registry.EntityAt(index), movement);
                }

                movement.position.x += movement.velocity.x * delta;
                movement.position.y += movement.velocity.y * delta;

                if (movement.velocity.x != 0.0f) {
                    float decel = FRICTION * delta;
                    if (std::abs(movement.velocity.x) <= decel) movement.velocity.x = 0.0f;
                    else movement.velocity.x -= decel * Sign(movement.velocity.x);
                }
            });
        }

        static float GetSpeedMultiplier(ComponentRegistry& registry, EntityID id) {
            HealthComponent* health = registry.GetComponent<HealthComponent>(id);
            return health ? health->SpeedMultiplier() : 1.0f;
        }

    private:
        void SteerTowardWaypoint(ComponentRegistry& registry, EntityID id, MovementComponent& movement) {
            Vector2 target = movement.waypoints.front();
            float dx = target.x - movement.position.x;

            if (std::abs(dx) <= WAYPOINTARRIVALRADIUS) {
                movement.waypoints.pop_front();
                return;
            }

            float speedmult = GetSpeedMultiplier(registry, id);
            movement.velocity.x = (dx > 0.0f ? 1.0f : -1.0f) * MOVESPEED * speedmult;
        }

        static float Sign(float value) {
            if (value > 0.0f) return 1.0f;
            if (value < 0.0f) return -1.0f;
            return 0.0f;
        }
};