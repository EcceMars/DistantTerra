#pragma once

#include <tuple>

#include "../registries/ComponentRegistry.hpp"

template <typename... SystemTypes>
class SystemRegistry {
    public:
        void UpdateAll(ComponentRegistry& registry, float delta) {
            (std::get<SystemTypes>(systems).Update(registry, delta), ...);
        }

        template <typename T>
        T& Get() { return std::get<T>(systems); }

    private:
        std::tuple<SystemTypes...> systems;
};