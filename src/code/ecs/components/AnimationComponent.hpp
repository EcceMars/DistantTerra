#pragma once

#include <deque>
#include <string>

#include "../../resources/AnimationResource.hpp"
#include "../systems/animation_system/AnimationLibrary.hpp"

struct AnimationFrameRef {
    const AnimationResource* resource = nullptr;
    Rectangle rect{};
};

class AnimationComponent {
    public:
        AnimationComponent(const AnimationLibrary* library, std::string defaultName)
            : library(library), defaultName(std::move(defaultName)) {}

        bool Play(const std::string& name) {
            if (!HasAnimation(name)) return false;

            if (queue.empty()) {
                queue.push_back(name);
                frame = 0;
                timer = 0.0f;
                return true;
            }
            if (queue.front() == name) return false;

            const AnimationClip* frontClip = library->GetClip(queue.front());
            if (frontClip && !frontClip->interruptible) return false;

            queue.front() = name;
            frame = 0;
            timer = 0.0f;
            return true;
        }

        // Same "unconditional, right now" contract as before, EXCEPT: firing
        // frame-0 events needs a registry, which this component doesn't have.
        // AnimationSystem checks forcedEventsPending on its next Update and
        // fires them then — a one-frame delay that didn't exist in the old
        // Entity-owned version. Flagging in case anything relies on a Force()'d
        // event landing same-frame; if that ever matters, it's a sign this
        // needs to move into AnimationSystem as a real method instead.
        void Force(const std::string& name) {
            if (!HasAnimation(name)) return;
            queue.clear();
            queue.push_back(name);
            frame = 0;
            timer = 0.0f;
            forcedEventsPending = true;
        }

        void Queue(const std::string& name, bool accumulates = false) {
            if (!HasAnimation(name)) return;
            if (!queue.empty() && !accumulates && queue.back() == name) return;
            queue.push_back(name);
        }

        void ClearQueue() {
            if (queue.empty()) return;
            std::string keep = queue.front();
            queue.clear();
            queue.push_back(keep);
        }

        bool HasAnimation(const std::string& name) const { return library->HasClip(name); }

        AnimationFrameRef CurrentFrame() const {
            if (queue.empty()) return {};
            const AnimationClip* clip = library->GetClip(queue.front());
            if (!clip || clip->frames.empty()) return {};
            return { clip->resource, clip->resource->GetFrameRect(clip->frames[frame]) };
        }

        const AnimationLibrary* library;
        std::string defaultName;
        std::deque<std::string> queue;
        int frame = 0;
        float timer = 0.0f;
        bool active = true;
        bool forcedEventsPending = false;
};