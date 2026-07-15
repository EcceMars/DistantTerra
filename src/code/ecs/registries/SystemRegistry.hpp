#pragma once

#include <tuple>
#include <utility>

#include "ComponentRegistry.hpp"

template <typename... SystemTypes>
class SystemRegistry {
    public:
        SystemRegistry() = default;

        template <typename... Args>
        explicit SystemRegistry(Args&&... args) : systems(std::forward<Args>(args)...) {}

        SystemRegistry(const SystemRegistry&) = delete;
        SystemRegistry& operator=(const SystemRegistry&) = delete;
        SystemRegistry(SystemRegistry&&) = delete;
        SystemRegistry& operator=(SystemRegistry&&) = delete;

        void InitAll() { (CallInit<SystemTypes>(), ...); }

        void UpdateAll(ComponentRegistry& registry, float delta) {
            (std::get<SystemTypes>(systems).Update(registry, delta), ...);
        }

        template <typename T>
        T& Get() { return std::get<T>(systems); }

    private:
        template <typename T>
        void CallInit() {
            if constexpr (requires { std::get<T>(systems).Init(*this); })
                std::get<T>(systems).Init(*this);
        }

        std::tuple<SystemTypes...> systems;
};