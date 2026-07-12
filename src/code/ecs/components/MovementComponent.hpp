#pragma once

#include <iostream>

#include "raylib.h"
#include "BaseComponent.hpp"

class MovementComponent : public BaseComponent {
    public:
        Vector2 position{0, 0};
        Vector2 velocity{0, 0};

        void Update(float delta) {
            velocity.y += GRAVITYBASE * delta;
            
            position.x += velocity.x * delta;
            position.y += velocity.y * delta;

            if (velocity.x != 0.0f) {
                float decel = FRICTION * delta;
                if ( abs(velocity.x) <= decel) {
                    velocity.x = 0.0f;
                } else {
                    velocity.x -= decel * Sign(velocity.x);
                }
            }
        };

    private:
        static float Sign(float value) {
            if (value > 0.0f) return 1.0f;
            if (value < 0.0f) return -1.0f;
            return 0.0f;
        }
};
