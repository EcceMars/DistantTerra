#pragma once

#include "raylib.h"
#include "BehaviorNode.hpp"
#include "../../registries/ComponentRegistry.hpp"
#include "../../systems/CommandSystem.hpp"

class WanderBehavior : public BehaviorNode {
    public:
        Status Tick(float delta, EntityID id, ComponentRegistry& registry, CommandSystem& actions) override {
            timer -= delta;
            if (timer <= 0.0f) PickNewDirection();

            if (direction != 0.0f) actions.Move(id, registry, direction, 0.35f);
            // direction == 0: an intentional pause — no Move() call, existing velocity just decays

            return Status::RUNNING;
        }

        void Reset() override { direction = 0.0f; timer = 0.0f; }

    private:
        static constexpr float MININTERVAL = 1.0f;
        static constexpr float MAXINTERVAL = 3.0f;

        float direction = 0.0f;
        float timer = 0.0f;

        void PickNewDirection() {
            direction = (float)(GetRandomValue(-1, 1));
            timer = (float)GetRandomValue((int)(MININTERVAL * 100), (int)(MAXINTERVAL * 100)) / 100.0f;
        }
};