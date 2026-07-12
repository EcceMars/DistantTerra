// at code/ecs/VisualComponent.hpp

#pragma once

#include "raylib.h"
#include "BaseComponent.hpp"
#include "MovementComponent.hpp"

class VisualComponent : public BaseComponent {
    public:
        VisualComponent(Texture2D* texture, Rectangle src_rect) : texture(texture), src_rect(src_rect) {}

        void Init() override {
            movement = owner->GetComponent<MovementComponent>();
        }

        void SetFacingRight(bool value) {
            row = value ? 0 : 1;
        }

        void Draw() override {
            if (!movement) return;

            Rectangle src = src_rect;
            src.y = row * src_rect.height;

            Rectangle dest = {
                movement->position.x,
                movement->position.y,
                src_rect.width * RENDERSCALE,
                src_rect.height * RENDERSCALE
            };
            DrawTexturePro(*texture, src, dest, {0, 0}, 0.0f, WHITE);
        }

    private:
        Texture2D* texture;
        Rectangle src_rect;
        MovementComponent* movement = nullptr;
        int row = 0;
};