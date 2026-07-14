#pragma once

#include <cstdint>
#include <functional>

constexpr uint32_t INVALID_GENERATION = 0xFFFFFFFFu;

struct EntityID {
    uint32_t index = 0;
    uint32_t generation = INVALID_GENERATION;
    
    bool IsValid() const {  return generation != INVALID_GENERATION; }

    bool operator==(const EntityID& other) const {  return index == other.index && generation == other.generation;  }
    bool operator!=(const EntityID& other) const {  return !(*this == other);   }
};

namespace std {
    template<> struct hash<EntityID> {
        size_t operator()(const EntityID& id) const noexcept {
            return (static_cast<size_t>(id.index) << 32) ^id.generation;
        }
    };
}