#pragma once

#include "raylib.h"
#include "BehaviorNode.hpp"
#include "../../Entity.hpp"
#include "../MovementComponent.hpp"
#include "../VisualComponent.hpp"
#include "../ColliderComponent.hpp"

class ControllerBehavior : public BehaviorNode {
    public:
        Status Tick(float delta, Entity* owner) {
            MovementComponent* movement = owner->GetComponent<MovementComponent>();
            if (!movement) return Status::FAILURE;

            ColliderComponent* collider = owner->GetComponent<ColliderComponent>();
            VisualComponent* visual = owner->GetComponent<VisualComponent>();

            bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
            bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

            if (left && !right) {
                movement->velocity.x = - MOVESPEED;
                if (visual) visual->SetFacingRight(false);
            }
            else if (right && !left) {
                movement->velocity.x = MOVESPEED;
                if (visual) visual->SetFacingRight(true);
            }

            if (collider && collider->grounded && IsKeyPressed(KEY_SPACE)) movement->velocity.y =  JUMPVELOCITY;

            return Status::RUNNING;
        }
};