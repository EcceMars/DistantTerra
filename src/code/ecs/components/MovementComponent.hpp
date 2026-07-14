#pragma once

#include "raylib.h"
#include "BaseComponent.hpp"

class MovementComponent : public BaseComponent {
    public:
        Vector2 position{0, 0};
        Vector2 velocity{0, 0};
};