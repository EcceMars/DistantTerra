#pragma once

#include "BehaviorStatus.hpp"

class Entity;

class BehaviorNode {
    public:
        virtual ~BehaviorNode() = default;
        virtual Status Tick(float delta, Entity* owner) = 0;
};