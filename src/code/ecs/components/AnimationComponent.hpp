#pragma once

#include <deque>
#include <string>

#include "BaseComponent.hpp"
#include "animation_system/AnimationLibrary.hpp"
#include "../../GLOBALS.hpp"

using namespace GLOBALS;

class AnimationComponent : public BaseComponent {
    public:
        AnimationComponent(const AnimationLibrary* library, std::string defaultName)
            : library(library), defaultName(std::move(defaultName)) {}

        // Weak swap: only replaces what's playing if it's the same clip already
        // (no-op, keeps playing), or the current clip is interruptible.
        // Safe to call every tick from a behavior — e.g. Play("walk") each frame
        // the entity is moving.
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

        // Unconditional: clears everything queued, plays this now, no exceptions.
        // This is what a hurt/blocked/death trigger calls.
        void Force(const std::string& name) {
            if (!HasAnimation(name)) return;

            queue.clear();
            queue.push_back(name);
            frame = 0;
            timer = 0.0f;

            FireEvents(library->GetClip(name), frame);
        }

        // Appends to the end. Only advances to once whatever's currently
        // playing finishes naturally (loops block this until interrupted).
        void Queue(const std::string& name, bool accumulates = false) {
            if (!HasAnimation(name)) return;
            if (!queue.empty() && !accumulates && queue.back() == name) return;
            queue.push_back(name);
        }

        // Drops everything pending, but leaves whatever's currently playing
        // alone so it doesn't cut mid-frame. Force() is the "stop right now" tool.
        void ClearQueue() {
            if (queue.empty()) return;
            std::string keep = queue.front();
            queue.clear();
            queue.push_back(keep);
        }

        void SetActive(bool value) { active = value; }
        bool IsActive() const { return active; }

        bool HasAnimation(const std::string& name) const { return library->HasClip(name); }

        AnimationFrame CurrentFrame() const {
            if (queue.empty()) return {0, 0};
            const AnimationClip* clip = library->GetClip(queue.front());
            if (!clip || clip->frames.empty()) return {0, 0};
            return clip->frames[frame];
        }

        void Update(float delta) override {
            if (!active) return;

            if (queue.empty()) {
                queue.push_back(defaultName);
                frame = 0;
                timer = 0.0f;
            }

            timer += delta;
            while (timer >= ANIMATIONFRAMEDURATION) {
                timer -= ANIMATIONFRAMEDURATION;
                Advance();
            }
        }

    private:
        void Advance() {
            const AnimationClip* clip = library->GetClip(queue.front());
            if (!clip || clip->frames.empty()) return;

            int nextFrame = frame + 1;
            if (nextFrame >= (int)clip->frames.size()) {
                if (clip->loop && queue.size() == 1) {
                    frame = 0; // still the only thing queued, keep looping
                } else {
                    queue.pop_front(); // one-shot done, or a looper got cut by something queued behind it
                    if (queue.empty()) queue.push_back(defaultName);
                    frame = 0;
                }
            } else {
                frame = nextFrame;
            }

            FireEvents(library->GetClip(queue.front()), frame);
        }

        void FireEvents(const AnimationClip* clip, int atFrame) {
            if (!clip) return;
            for (auto& [idx, fn] : clip->events) {
                if (idx == atFrame && fn) fn(owner);
            }
        }

        const AnimationLibrary* library;
        std::string defaultName;
        std::deque<std::string> queue;
        int frame = 0;
        float timer = 0.0f;
        bool active = true;
};