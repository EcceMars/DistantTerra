#pragma once

#include "../registries/ComponentRegistry.hpp"

class BaseSystem {
    public:
        virtual ~BaseSystem() = default;
        virtual void Update(ComponentRegistry&, float) {}
};