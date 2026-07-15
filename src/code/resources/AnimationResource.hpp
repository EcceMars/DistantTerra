#pragma once

#include <string>

#include "raylib.h"
#include "../stores/SpriteStore.hpp"

struct AnimationResource {
    AnimationResource(SpriteStore& store, const std::string& path, int framewidth, int frameheight, int cols, int rows)
        : texture(store.GetTexture(path)), framewidth(framewidth), frameheight(frameheight), cols(cols), rows(rows) {}

    AnimationResource(Texture2D* texture, int framewidth, int frameheight, int cols, int rows)
        : texture(texture), framewidth(framewidth), frameheight(frameheight), cols(cols), rows(rows) {}

    Texture2D* texture;
    int framewidth, frameheight;
    int cols, rows;

    Rectangle GetFrameRect(int frameindex) const {
        int col = frameindex % cols;
        int row = frameindex / cols;
        return Rectangle{ (float)(col * framewidth), (float)(row * frameheight), (float)framewidth, (float)frameheight };
    }
};