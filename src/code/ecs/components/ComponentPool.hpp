#pragma once

#include <vector>
#include <optional>
#include <utility>

#include "../entities/EntityID.hpp"

template <typename T>
class ComponentPool {
    public:
        template <typename... Args>
        T& Add(EntityID id, Args&&... args) {
            EnsureSize(id.index);
            data[id.index].emplace(std::forward<Args>(args)...);
            return *data[id.index];
        }

        void Remove(EntityID id) {  if (id.index < data.size()) data[id.index].reset(); }

        T* Get(EntityID id) {
            if (id.index >= data.size() || !data[id.index]) return nullptr;
            return &*data[id.index];
        }

        bool Has(EntityID id) const {
            return id.index < data.size() && data[id.index].has_value();
        }
        
        template <typename Fn>
        void ForEach(Fn&& fn) {
            for (uint32_t i = 0; i < data.size(); i++) {
                if (data[i]) fn(i, *data[i]);
            }
        }
    private:
        void EnsureSize(uint32_t index) {
            if (index >= data.size()) data.resize(index + 1);
        }

        std::vector<std::optional<T>> data;
};