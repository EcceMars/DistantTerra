#pragma once

#include "raylib.h"
#include "BehaviorNode.hpp"
#include "../../Entity.hpp"
#include "../MovementComponent.hpp"

// Picks a random horizontal direction, holds it for a while, then picks again.
// Mirrors ControllerBehavior's shape but drives itself instead of reading input.
class WanderBehavior : public BehaviorNode {
    public:
        Status Tick(float delta, Entity* owner) override {
            MovementComponent* movement = owner->GetComponent<MovementComponent>();
            if (!movement) return Status::FAILURE;

            timer -= delta;
            if (timer <= 0.0f) {
                PickNewDirection();
            }

            movement->velocity.x = direction * WANDERSPEED * movement->SpeedMultiplier();

            return Status::RUNNING;
        }

    private:
        static constexpr float WANDERSPEED = MOVESPEED * 0.35f;
        static constexpr float MININTERVAL = 1.0f;
        static constexpr float MAXINTERVAL = 3.0f;

        float direction = 0.0f;
        float timer = 0.0f;

        void PickNewDirection() {
            // -1, 0, or 1 — the 0 case gives the slime a chance to actually pause
            direction = (float)(GetRandomValue(-1, 1));
            timer = (float)GetRandomValue((int)(MININTERVAL * 100), (int)(MAXINTERVAL * 100)) / 100.0f;
        }
};