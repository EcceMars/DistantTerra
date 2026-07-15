#include <algorithm>
#include <memory>

#include "raylib.h"
#include "code/GLOBALS.hpp"
#include "code/stores/SpriteStore.hpp"
#include "code/world/World.hpp"
#include "code/ecs/registries/ComponentRegistry.hpp"
#include "code/ecs/registries/SystemRegistry.hpp"
#include "code/ecs/systems/BehaviorSystem.hpp"
#include "code/ecs/systems/CommandSystem.hpp"
#include "code/ecs/systems/MovementSystem.hpp"
#include "code/ecs/systems/CollisionSystem.hpp"
#include "code/ecs/systems/AnimationSystem.hpp"
#include "code/ecs/systems/WorldSystem.hpp"
#include "code/ecs/systems/RenderSystem.hpp"
#include "code/resources/AnimationResource.hpp"
#include "code/ecs/components/behaviors/ControllerBehavior.hpp"
#include "code/ecs/components/behaviors/WanderBehavior.hpp"

using namespace GLOBALS;

// Both sheets share the same row layout: 0=idle_right, 1=idle_left,
// 2=walk_right, 3=walk_left. Trailing rows (slime's die animation) are just
// never referenced by any clip — no need to represent "unused" explicitly.
void AddDirectionalClips(AnimationLibrary& library, const AnimationResource* atlas) {
    library.AddClip(MakeRangeClip("idle_right", atlas, 0, atlas->cols - 1));
    library.AddClip(MakeRangeClip("idle_left",  atlas, atlas->cols, atlas->cols * 2 - 1));
    library.AddClip(MakeRangeClip("walk_right", atlas, atlas->cols * 2, atlas->cols * 3 - 1));
    library.AddClip(MakeRangeClip("walk_left",  atlas, atlas->cols * 3, atlas->cols * 4 - 1));
}

int main() {
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Distant: Terra");
    // icon + resizable flag: yours
    SetTargetFPS(60);

    World world;
    SpriteStore spriteresources;

    constexpr int floortiley = WORLDTILEHIGH - 2;
    for (int tx = -5; tx <= WORLDTILEWIDE + 5; tx++)
        for (int ty = floortiley; ty < floortiley + 3; ty++)
            world.GetTile(tx, ty).type = TileType::GRASS;

    AnimationResource iphritatlas(spriteresources, SPRITEPATH + "iphrit.png", 32, 32, 4, 4);
    AnimationLibrary iphritlibrary;
    AddDirectionalClips(iphritlibrary, &iphritatlas);

    AnimationResource slimeatlas(spriteresources, SPRITEPATH + "green_slime.png", 16, 16, 4, 6);
    AnimationLibrary slimelibrary;
    AddDirectionalClips(slimelibrary, &slimeatlas); // rows 4-5 (die) simply unreferenced

    ComponentRegistry registry;
    SystemRegistry<BehaviorSystem, MovementSystem, CollisionSystem, AnimationSystem, WorldSystem, RenderSystem, CommandSystem> systems{
        BehaviorSystem{}, MovementSystem{}, CollisionSystem{ &world }, AnimationSystem{}, WorldSystem{ &world }, RenderSystem{}, CommandSystem{}
    };
    systems.InitAll();

    EntityID player = registry.CreateEntity();
    MovementComponent* playerMove = registry.AddComponent<MovementComponent>(player);
    playerMove->position = { SCREENWIDTH * 0.5f, SCREENHEIGHT * 0.25f };
    registry.AddComponent<ColliderComponent>(player, PLAYERWIDTH, PLAYERHEIGHT);
    registry.AddComponent<VisualComponent>(player, &iphritatlas);
    registry.AddComponent<AnimationComponent>(player, &iphritlibrary, "idle_right");
    registry.AddComponent<WorldComponent>(player, 2);
    registry.AddComponent<BehaviorComponent>(player)->SetRoot(std::make_unique<ControllerBehavior>());

    // Second entity, different atlas, different behavior root — exercises the
    // same chain (Behavior -> Movement/Collision -> Animation -> Render)
    // through an entirely independent set of component instances.
    EntityID slime = registry.CreateEntity();
    MovementComponent* slimeMove = registry.AddComponent<MovementComponent>(slime);
    slimeMove->position = { SCREENWIDTH * 0.35f, SCREENHEIGHT * 0.25f };
    registry.AddComponent<ColliderComponent>(slime, SLIMEWIDTH, SLIMEHEIGHT);
    registry.AddComponent<VisualComponent>(slime, &slimeatlas);
    registry.AddComponent<AnimationComponent>(slime, &slimelibrary, "idle_right");
    registry.AddComponent<BehaviorComponent>(slime)->SetRoot(std::make_unique<WanderBehavior>());

    while (!WindowShouldClose()) {
        float delta = std::min(GetFrameTime(), DELTALIMIT);
        systems.UpdateAll(registry, delta);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        systems.Get<RenderSystem>().Draw(registry);
        systems.Get<CollisionSystem>().DrawDebug(registry);
        EndDrawing();
    }

    spriteresources.UnloadAll();
    CloseWindow();
    return 0;
}