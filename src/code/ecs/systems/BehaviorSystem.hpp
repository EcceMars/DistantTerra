#pragma once

#include "BaseSystem.hpp"
#include "CommandSystem.hpp"
#include "../components/BehaviorComponent.hpp"

class BehaviorSystem : public BaseSystem {
    public:
        template <typename SystemRegistryT>
        void Init(SystemRegistryT& systems) {
            actions = &systems.template Get<CommandSystem>();
        }

        void Update(ComponentRegistry& registry, float delta) override {
            if (!actions) return;

            registry.ForEach<BehaviorComponent>([&](uint32_t index, BehaviorComponent& behavior) {
                if (BehaviorNode* root = behavior.GetRoot())
                    root->Tick(delta, registry.EntityAt(index), registry, *actions);
            });
        }

    private:
        CommandSystem* actions = nullptr;
};