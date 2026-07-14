// at code/ecs/VisualComponent.hpp

#pragma once

#include "raylib.h"
#include "BaseComponent.hpp"
#include "AnimationComponent.hpp"
#include "MovementComponent.hpp"

class VisualComponent : public BaseComponent {
    public:
        VisualComponent(Texture2D* texture, Rectangle src_rect) : texture(texture), src_rect(src_rect) {}

        void Init() override {
            animation = owner->GetComponent<AnimationComponent>();
            movement = owner->GetComponent<MovementComponent>();
        }

        void SetFacingRight(bool value) {
            row = value ? 0 : 1;
        }

        void SetGroundOffset(float offset = SPRITEUNIVERSALYOFFSET) {    groundoffset = offset;  }
        float GetGroundOffset() const { return groundoffset;    }

        void Draw() override {
            if (!movement) return;

            Rectangle src = src_rect;
            if (animation) {
                AnimationFrame f = animation->CurrentFrame();
                src.x = f.col * src_rect.width;
                src.y = f.row * src_rect.height;
            } else {
                src.y = row * src_rect.height;
            }

            float dest_width = src_rect.width * RENDERSCALE;
            float dest_height = src_rect.height * RENDERSCALE;

            Rectangle dest = {
                movement->position.x - dest_width * 0.5f,
                movement->position.y - dest_height + GetGroundOffset(),
                dest_width,
                dest_height
            };

            DrawTexturePro(*texture, src, dest, {0, 0}, 0.0f, WHITE);
        }

    private:
        Texture2D* texture;
        Rectangle src_rect;
        AnimationComponent* animation = nullptr;
        MovementComponent* movement = nullptr;
        int row = 0;
        float groundoffset = 0.0f;
};