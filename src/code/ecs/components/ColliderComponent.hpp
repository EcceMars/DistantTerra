#pragma once

// Pure data now. The old cached `world` pointer and `movement` pointer are both
// gone — CollisionSystem resolves the sibling MovementComponent fresh each tick
// (same pattern as RenderSystem), and takes World* once at construction instead
// of every component carrying its own copy of the same pointer.
struct ColliderComponent {
    ColliderComponent(float width, float height) : width(width), height(height) {}

    float width, height;
    bool grounded = false;
};