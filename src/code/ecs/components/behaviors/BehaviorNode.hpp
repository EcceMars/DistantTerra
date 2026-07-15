#pragma once

#include "BehaviorStatus.hpp"
#include "../../entities/EntityID.hpp"

class ComponentRegistry;
class CommandSystem;

class BehaviorNode {
    public:
        virtual ~BehaviorNode() = default;
        virtual Status Tick(float delta, EntityID id, ComponentRegistry& registry, CommandSystem& actions) = 0;
        virtual void Reset() {}
};