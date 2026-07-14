#pragma once

#include <vector>
#include <deque>

#include "EntityID.hpp"

class EntityAllocator {
    public:
        EntityID Create() {
            uint32_t index;
            if (!free_indices.empty()) {
                index = free_indices.front();
                free_indices.pop_front();
            } else {
                index = (uint32_t)generations.size();
                generations.push_back(0);
                alive.push_back(false);
            }
            alive[index] = true;
            return EntityID{    index, generations[index]   };
        }

        void Destroy(EntityID id) {
            if (!IsAlive(id)) return;

            alive[id.index] = false;
            generations[id.index]++;
            free_indices.push_back(id.index);
        }

        bool IsAlive(EntityID id) const {
            return id.IsValid()
                && id.index < alive.size()
                && alive[id.index]
                && generations[id.index] == id.generation;
        }

        EntityID EntityAt(uint32_t index) const {
            if (index >= alive.size() || !alive[index]) return EntityID{};
            return EntityID{ index, generations[index] };
        }
 
        uint32_t Capacity() const { return (uint32_t)generations.size(); }

    private:
        std::vector<uint32_t> generations;
        std::vector<bool> alive;
        std::deque<uint32_t> free_indices;
};