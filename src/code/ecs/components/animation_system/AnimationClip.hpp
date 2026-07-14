#pragma once

#include <functional>
#include <string>
#include <vector>

class Entity;

struct AnimationFrame {
    int col, row;
};

struct AnimationClip {
    std::string name;
    std::vector<AnimationFrame> frames;
    bool loop = true;
    bool interruptible = true;

    std::vector<std::pair<int, std::function<void(Entity*)>>> events;
};