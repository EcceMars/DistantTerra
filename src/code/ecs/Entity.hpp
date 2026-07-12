#pragma once

#include <vector>
#include <memory>

#include "components/BaseComponent.hpp"

class Entity {
    public:
        template <typename T, typename... Args>
        T* AddComponent(Args&&... args) {
            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            component->owner = this;
            T* raw = component.get();
            components.push_back(std::move(component));
            raw->Init();
            return raw;
        }

        template <typename T>
        T* GetComponent() {
            for (auto& component : components) {
                if (T* cast = dynamic_cast<T*>(component.get())) return cast;
            }
            return nullptr;
        }

        void Update(float delta) {
            for (auto& component : components) component->Update(delta);
        }

        void Draw() {
            for (auto& component : components) component->Draw();
        }

    private:
        std::vector<std::unique_ptr<BaseComponent>> components;
};