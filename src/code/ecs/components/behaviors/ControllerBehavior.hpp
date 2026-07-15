#pragma once

#include "raylib.h"
#include "BehaviorNode.hpp"
#include "../../registries/ComponentRegistry.hpp"
#include "../../systems/CommandSystem.hpp"

class ControllerBehavior : public BehaviorNode {
    public:
        Status Tick(float delta, EntityID id, ComponentRegistry& registry, CommandSystem& actions) override {
            bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
            bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

            if (left != right) actions.Move(id, registry, left ? -1.0f : 1.0f);
            if (IsKeyPressed(KEY_SPACE)) actions.Jump(id, registry);

            return Status::RUNNING;
        }
};