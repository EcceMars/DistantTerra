#pragma once

#include "../../GLOBALS.hpp"

using namespace GLOBALS;

class Entity;
class BaseComponent {
    public:
        virtual ~BaseComponent() = default;

        virtual void Init() {}
        virtual void Update(float delta) {}
        virtual void Draw() {}

        Entity* owner = nullptr;
};