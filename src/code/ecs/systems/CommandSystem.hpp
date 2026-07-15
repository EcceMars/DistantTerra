// code/ecs/systems/ActionSystem.hpp
#pragma once

#include <cmath>
#include <string>

#include "BaseSystem.hpp"
#include "MovementSystem.hpp"
#include "../components/MovementComponent.hpp"
#include "../components/VisualComponent.hpp"
#include "../components/AnimationComponent.hpp"
#include "../components/ColliderComponent.hpp"
#include "../../GLOBALS.hpp"

// The single point of contact between behaviors and everything else. Behaviors
// express intent — Turn/Move/Jump/MoveTo — without knowing how facing is
// encoded, what "grounded" means, or which clip names exist.
class CommandSystem : public BaseSystem {
    public:
        // --- Immediate verbs ---

        void Turn(EntityID id, ComponentRegistry& registry, bool facingright) {
            if (VisualComponent* visual = registry.GetComponent<VisualComponent>(id))
                visual->facingright = facingright;
        }

        void Move(EntityID id, ComponentRegistry& registry, float direction, float speedscale = 1.0f) {
            MovementComponent* movement = registry.GetComponent<MovementComponent>(id);
            if (!movement) return;

            movement->waypoints.clear(); // explicit control cancels any patrol in progress
            float speedmult = MovementSystem::GetSpeedMultiplier(registry, id);
            movement->velocity.x = direction * MOVESPEED * speedmult * speedscale;
        }

        void Jump(EntityID id, ComponentRegistry& registry) {
            MovementComponent* movement = registry.GetComponent<MovementComponent>(id);
            ColliderComponent* collider = registry.GetComponent<ColliderComponent>(id);
            if (!movement || !collider || !collider->grounded) return;

            movement->velocity.y = JUMPVELOCITY;
        }

        // --- Delayed verb: enqueue only, MovementSystem does the chasing ---

        void MoveTo(EntityID id, ComponentRegistry& registry, Vector2 point) {
            if (MovementComponent* movement = registry.GetComponent<MovementComponent>(id))
                movement->waypoints.push_back(point);
        }

        // --- Per-frame sync: facing/animation follow whatever velocity ended up
        // being this tick, regardless of what set it (input, wander, waypoint chase) ---

        void Update(ComponentRegistry& registry, float delta) override {
            registry.ForEach<VisualComponent>([&](uint32_t index, VisualComponent& visual) {
                EntityID id = registry.EntityAt(index);
                MovementComponent* movement = registry.GetComponent<MovementComponent>(id);
                if (!movement) return;

                constexpr float MOVETHRESHOLD = 5.0f; // ignore friction's tail-end trickle
                bool moving = std::abs(movement->velocity.x) > MOVETHRESHOLD;

                // Only re-derive facing while actually moving — an explicit Turn()
                // while stationary (facing a target before an attack, later)
                // shouldn't get clobbered by residual near-zero velocity.
                if (moving) visual.facingright = movement->velocity.x > 0.0f;

                AnimationComponent* anim = registry.GetComponent<AnimationComponent>(id);
                if (!anim) return;

                std::string suffix = visual.facingright ? "_right" : "_left";
                anim->Play((moving ? "walk" : "idle") + suffix);
            });
        }
};