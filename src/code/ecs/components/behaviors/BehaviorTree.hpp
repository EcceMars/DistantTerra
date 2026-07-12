#pragma once

#include <vector>
#include <memory>

#include "BehaviorNode.hpp"

// Goes through each child and selects the first which isn't a Status::FAILURE—low-priority behavior
class SelectorNode : public BehaviorNode {
    public:
        SelectorNode& AddChild(std::unique_ptr<BehaviorNode> child) {
            children.push_back(std::move(child));
            return *this;
        }

        Status Tick(float delta, Entity* owner) override {
            for (auto& child : children) {
                Status status = child->Tick(delta, owner);
                if (status != Status::FAILURE) return status;
            }
            return Status::FAILURE;
        }

    private:
        std::vector<std::unique_ptr<BehaviorNode>> children;
};

// Checks a sequence of behaviors and fails at the first Stauts::FAILURE, otherwise returns Status::SUCCESS
class SequenceNode : public BehaviorNode {
    public:
        SequenceNode& AddChild(std::unique_ptr<BehaviorNode> child) {
            children.push_back(std::move(child));
            return *this;
        }

        Status Tick(float delta, Entity* owner) override {
            for (auto& child : children) {
                Status status = child->Tick(delta, owner);
                if (status != Status::SUCCESS) return status;
            }
            return Status::SUCCESS;
        }

    private:
        std::vector<std::unique_ptr<BehaviorNode>> children;
};