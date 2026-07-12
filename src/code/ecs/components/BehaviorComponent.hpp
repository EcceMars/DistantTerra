#pragma once

#include <memory>

#include "BaseComponent.hpp"
#include "behaviors/BehaviorNode.hpp"

class BehaviorComponent : public BaseComponent {
    public:
        void SetRoot(std::unique_ptr<BehaviorNode> node) {
            root = std::move(node);
        }

        void Update(float delta) override {
            if (root) root->Tick(delta, owner);
        }
    private:
        std::unique_ptr<BehaviorNode> root;
};