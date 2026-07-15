#pragma once

#include "raylib.h"

struct WorldComponent {
    WorldComponent(int radius = 2) : radius(radius) {}

    int radius;

    // Only read when the entity has no MovementComponent — a static area like
    // a village anchor. If MovementComponent IS present, WorldSystem tracks
    // that position instead and this is ignored.
    Vector2 anchor{0, 0};
};