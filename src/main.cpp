#include <cassert>
#include <cmath>
#include <iostream>

#include "code/ecs/registries/ComponentRegistry.hpp"
#include "code/ecs/systems/SystemRegistry.hpp"
#include "code/ecs/systems/MovementSystem.hpp"

int main() {
    ComponentRegistry registry;
    SystemRegistry<MovementSystem> systems;

    // Entity with movement + an agility-boosted health component
    EntityID hero = registry.CreateEntity();
    registry.AddComponent<MovementComponent>(hero);
    HealthComponent* heroHealth = registry.AddComponent<HealthComponent>(hero);
    heroHealth->AddAttribute(Attributes::AGILITY, 21); // 1 + (21-1)*0.025 = 1.5

    // Entity with movement only, no health at all
    EntityID rock = registry.CreateEntity();
    registry.AddComponent<MovementComponent>(rock);

    constexpr float delta = 1.0f / 60.0f;
    systems.UpdateAll(registry, delta);

    // Gravity should have integrated once for BOTH entities, independently
    MovementComponent* heroMove = registry.GetComponent<MovementComponent>(hero);
    MovementComponent* rockMove = registry.GetComponent<MovementComponent>(rock);
    float expectedVelY = GRAVITYBASE * delta;
    assert(std::abs(heroMove->velocity.y - expectedVelY) < 0.001f);
    assert(std::abs(rockMove->velocity.y - expectedVelY) < 0.001f);
    assert(std::abs(heroMove->position.y - expectedVelY * delta) < 0.001f);

    // Cross-component lookup during a ForEach pass, mirroring _get_vitality(uid)
    // inside _separate(): walk the pool, resolve each slot's live EntityID,
    // then ask the registry for that same entity's HealthComponent.
    int checked = 0;
    registry.ForEach<MovementComponent>([&](uint32_t index, MovementComponent&) {
        EntityID id = registry.EntityAt(index);
        float mult = MovementSystem::GetSpeedMultiplier(registry, id);
        if (id == hero) assert(std::abs(mult - 1.5f) < 0.001f);
        if (id == rock) assert(std::abs(mult - 1.0f) < 0.001f); // no HealthComponent -> neutral
        checked++;
    });
    assert(checked == 2);

    // Generation safety: destroy hero, recreate on the same slot without health,
    // and make sure the OLD handle can't pull the new entity's (lack of) data.
    EntityID staleHero = hero;
    registry.DestroyEntity(hero);
    EntityID impostor = registry.CreateEntity();
    registry.AddComponent<MovementComponent>(impostor);
    assert(impostor.index == staleHero.index);       // slot reused
    assert(impostor.generation != staleHero.generation);

    float staleMult = MovementSystem::GetSpeedMultiplier(registry, staleHero);
    assert(staleMult == 1.0f); // stale handle resolves to nothing, not to impostor's data

    std::cout << "All movement/system integration tests passed.\n";
    return 0;
}