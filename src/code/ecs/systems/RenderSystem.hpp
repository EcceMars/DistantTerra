#pragma once

#include "raylib.h"
#include "BaseSystem.hpp"
#include "../components/MovementComponent.hpp"
#include "../components/VisualComponent.hpp"
#include "../components/AnimationComponent.hpp"
#include "../../GLOBALS.hpp"

class RenderSystem : public BaseSystem {
    public:
        void Draw(ComponentRegistry& registry) {
            registry.ForEach<VisualComponent>([&](uint32_t index, VisualComponent& visual) {
                EntityID id = registry.EntityAt(index);
                MovementComponent* movement = registry.GetComponent<MovementComponent>(id);
                if (!movement) return;

                const AnimationResource* resource = visual.resource;
                Rectangle src;

                AnimationComponent* anim = registry.GetComponent<AnimationComponent>(id);
                if (anim) {
                    AnimationFrameRef ref = anim->CurrentFrame();
                    if (!ref.resource) return;
                    resource = ref.resource;
                    src = ref.rect;
                } else {
                    int frameindex = visual.facingright ? visual.staticframeright : visual.staticframeleft;
                    src = resource->GetFrameRect(frameindex);
                }

                float destwidth = resource->framewidth * RENDERSCALE;
                float destheight = resource->frameheight * RENDERSCALE;

                Rectangle dest{
                    movement->position.x - destwidth * 0.5f,
                    movement->position.y - destheight + visual.groundoffset,
                    destwidth,
                    destheight
                };

                DrawTexturePro(*resource->texture, src, dest, {0, 0}, 0.0f, WHITE);
            });
        }
};