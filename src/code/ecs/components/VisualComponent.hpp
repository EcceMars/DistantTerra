#pragma once

#include "raylib.h"
#include "../../resources/AnimationResource.hpp"
#include "../../GLOBALS.hpp"

struct VisualComponent {
    VisualComponent(const AnimationResource* resource, int staticframeright = 0, int staticframeleft = 0)
        : resource(resource), staticframeright(staticframeright), staticframeleft(staticframeleft) {}

    const AnimationResource* resource;

    // Fallback frames when there's no AnimationComponent (a static prop).
    // Ignored once AnimationComponent exists — facing there is baked into
    // which clip plays, per "no sprite flipping."
    int staticframeright, staticframeleft;

    bool facingright = true;
    float groundoffset = SPRITEUNIVERSALYOFFSET;
};