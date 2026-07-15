#pragma once

#include <memory>

#include "behaviors/BehaviorNode.hpp"

class BehaviorComponent {
    public:
        void SetRoot(std::unique_ptr<BehaviorNode> node) { root = std::move(node); }
        BehaviorNode* GetRoot() { return root.get(); }

    private:
        std::unique_ptr<BehaviorNode> root;
};