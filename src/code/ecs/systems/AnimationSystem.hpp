#pragma once

#include "BaseSystem.hpp"
#include "../components/AnimationComponent.hpp"
#include "../../GLOBALS.hpp"

using namespace GLOBALS;

class AnimationSystem : public BaseSystem {
    public:
        void Update(ComponentRegistry& registry, float delta) override {
            registry.ForEach<AnimationComponent>([&](uint32_t index, AnimationComponent& anim) {
                if (!anim.active) return;

                EntityID id = registry.EntityAt(index);

                if (anim.forcedEventsPending) {
                    FireEvents(anim, id, registry);
                    anim.forcedEventsPending = false;
                }

                if (anim.queue.empty()) {
                    anim.queue.push_back(anim.defaultName);
                    anim.frame = 0;
                    anim.timer = 0.0f;
                }

                anim.timer += delta;
                while (anim.timer >= ANIMATIONFRAMEDURATION) {
                    anim.timer -= ANIMATIONFRAMEDURATION;
                    Advance(anim, id, registry);
                }
            });
        }

    private:
        void Advance(AnimationComponent& anim, EntityID id, ComponentRegistry& registry) {
            const AnimationClip* clip = anim.library->GetClip(anim.queue.front());
            if (!clip || clip->frames.empty()) return;

            int nextFrame = anim.frame + 1;
            if (nextFrame >= (int)clip->frames.size()) {
                if (clip->loop && anim.queue.size() == 1) {
                    anim.frame = 0;
                } else {
                    anim.queue.pop_front();
                    if (anim.queue.empty()) anim.queue.push_back(anim.defaultName);
                    anim.frame = 0;
                }
            } else {
                anim.frame = nextFrame;
            }

            FireEvents(anim, id, registry);
        }

        void FireEvents(AnimationComponent& anim, EntityID id, ComponentRegistry& registry) {
            const AnimationClip* clip = anim.library->GetClip(anim.queue.front());
            if (!clip) return;
            for (auto& [idx, fn] : clip->events) {
                if (idx == anim.frame && fn) fn(id, registry);
            }
        }
};