#pragma once

#include <functional>
#include <memory>

#include "BehaviorNode.hpp"

// Gates `child` behind `condition`. Returns FAILURE (not ABORTED) when the
// condition is false — matches the C++ four-state model: the gate itself
// didn't fail to run, the behavior just isn't applicable, which is what a
// SelectorNode's "try the next option" check (status != FAILURE) expects.
class BehaviorCondition : public BehaviorNode {
    public:
        using ConditionFn = std::function<bool(EntityID, ComponentRegistry&)>;

        BehaviorCondition(ConditionFn condition, std::unique_ptr<BehaviorNode> child)
            : condition(std::move(condition)), child(std::move(child)) {}

        Status Tick(float delta, EntityID id, ComponentRegistry& registry) override {
            if (!condition(id, registry)) return Status::FAILURE;
            return child->Tick(delta, id, registry);
        }

        void Reset() override { child->Reset(); }

    private:
        ConditionFn condition;
        std::unique_ptr<BehaviorNode> child;
};