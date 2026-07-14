#pragma once

#include <tuple>

#include "../entities/EntityAllocator.hpp"
#include "../components/ComponentPool.hpp"

template <typename... ComponentType>
class Registry {
    public:
        EntityID CreateEntity() {   return allocator.Create();   }

        void DestroyEntity(EntityID id) {
            if (!allocator.IsAlive(id)) return;

            (std::get<ComponentPool<ComponentType>>(pools).Remove(id), ...);
            allocator.Destroy(id);
        }

        bool IsAlive(EntityID id) const { return allocator.IsAlive(id); }

        EntityID EntityAt(uint32_t index) const { return allocator.EntityAt(index); }

        template <typename T, typename... Args>
        T* AddComponent(EntityID id, Args&&... args) {
            if (!allocator.IsAlive(id)) return nullptr;
            return &std::get<ComponentPool<T>>(pools).Add(id, std::forward<Args>(args)...);
        }

        template <typename T>
        T* GetComponent(EntityID id) {
            return std::get<ComponentPool<T>>(pools).Get(id);
        }

        template <typename T>
        bool HasComponent(EntityID id) const {
            if (!allocator.IsAlive(id)) return false;
            return std::get<ComponentPool<T>>(pools).Has(id);
        }

        template <typename T>
        void RemoveComponent(EntityID id) {
            if (!allocator.IsAlive(id)) return;
            std::get<ComponentPool<T>>(pools).Remove(id);
        }

        template <typename T, typename Fn>
        void ForEach(Fn&& fn) {
            std::get<ComponentPool<T>>(pools).ForEach(std::forward<Fn>(fn));
        }
    private:
        EntityAllocator allocator;
        std::tuple<ComponentPool<ComponentType>...> pools;
};