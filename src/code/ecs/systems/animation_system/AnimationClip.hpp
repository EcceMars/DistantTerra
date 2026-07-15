#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../../../resources/AnimationResource.hpp"
#include "../../entities/EntityID.hpp"

class ComponentRegistry;

struct AnimationClip {
    std::string name;
    const AnimationResource* resource;
    std::vector<int> frames; // flat indices into resource, in playback order
    bool loop = true;
    bool interruptible = true;

    std::vector<std::pair<int, std::function<void(EntityID, ComponentRegistry&)>>> events;
};

inline AnimationClip MakeClip(std::string name, const AnimationResource* resource, std::vector<int> frames,
                               bool loop = true, bool interruptible = true) {
    AnimationClip clip;
    clip.name = std::move(name);
    clip.resource = resource;
    clip.frames = std::move(frames);
    clip.loop = loop;
    clip.interruptible = interruptible;
    return clip;
}

// Replaces the old MakeRowClip — "row" isn't a clip-level concept anymore,
// but contiguous frame ranges are still the common case.
inline AnimationClip MakeRangeClip(std::string name, const AnimationResource* resource, int startframe, int endframe,
                                    bool loop = true, bool interruptible = true) {
    std::vector<int> frames;
    for (int f = startframe; f <= endframe; f++) frames.push_back(f);
    return MakeClip(std::move(name), resource, std::move(frames), loop, interruptible);
}