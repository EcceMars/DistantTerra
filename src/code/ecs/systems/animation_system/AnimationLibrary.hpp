#pragma once

#include <unordered_map>
#include <string>
#include <utility>

#include "AnimationClip.hpp"

class AnimationLibrary {
    public:
        void AddClip(AnimationClip clip) {
            clips[clip.name] = std::move(clip);
        }

        const AnimationClip* GetClip(const std::string& name) const {
            auto it = clips.find(name);
            return it != clips.end() ? &it->second : nullptr;
        }

        bool HasClip(const std::string& name) const {
            return clips.find(name) != clips.end();
        }

    private:
        std::unordered_map<std::string, AnimationClip> clips;
};