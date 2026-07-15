#pragma once

#include <deque>

#include "raylib.h"

class MovementComponent : public BaseComponent {
    public:
        Vector2 position{0, 0};
        Vector2 velocity{0, 0};

        std::deque<Vector2> waypoints;
};