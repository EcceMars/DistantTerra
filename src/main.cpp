#include "raylib.h"
#include "code/GLOBALS.hpp"
#include "code/world/Tile.hpp"
#include "code/world/World.hpp"
#include "code/ecs/Entity.hpp"
#include "code/ecs/components/BehaviorComponent.hpp"
#include "code/ecs/components/MovementComponent.hpp"
#include "code/ecs/components/VisualComponent.hpp"
#include "code/ecs/components/WorldComponent.hpp"
#include "code/ecs/components/behaviors/BehaviorTree.hpp"
#include "code/ecs/components/behaviors/ControllerBehavior.hpp"
#include "code/ecs/components/behaviors/WanderBehavior.hpp"

#define ASSETSPATH "src/assets/"

Rectangle GetTileSourceRect(int col, int row) {
    return Rectangle {
        (float)(col * TILESIZE),
        (float)(row * TILESIZE),
        (float)TILESIZE,
        (float)TILESIZE
    };
}

void GenerateTestTerrain(World& world, int surfaceRow) {
    int stoneRow = surfaceRow + 6;

    for (int x = 0; x < WORLDTILEWIDE; x++) {
        for (int y = surfaceRow; y < WORLDTILEHIGH; y++) {
            Tile& tile = world.GetTile(x, y);
            if (y == surfaceRow) {
                tile = { TileType::GRASS, TileVariant::SURFACE };
            } else if (y < stoneRow) {
                tile = { TileType::SOIL, TileVariant::BURIED };
            } else {
                tile = { TileType::STONE, TileVariant::BURIED };
            }
        }
    }
}

void DrawTile(Texture2D& atlas, const Tile& tile, int gridx, int gridy) {
    if (tile.type == TileType::AIR) return;

    Rectangle src = GetTileSourceRect(GetTileColumn(tile.type), (int)tile.variant);
    Rectangle dest = {
        (float)(gridx * TILESIZE * RENDERSCALE),
        (float)(gridy * TILESIZE * RENDERSCALE),
        (float)(TILESIZE * RENDERSCALE),
        (float)(TILESIZE * RENDERSCALE)
    };

    DrawTexturePro(atlas, src, dest, {0, 0}, 0.0f, WHITE);
}

void DrawWorld(Texture2D& atlas, World& world) {
    for (auto& [coord, chunk] : world.GetChunks()) {
        if (!chunk.active) continue;

        for (int localy = 0; localy < Chunk::SIZE; localy++) for (int localx = 0; localx < Chunk::SIZE; localx++) {
            int worldx = coord.x * Chunk::SIZE + localx;
            int worldy = coord.y * Chunk::SIZE + localy;
            DrawTile(atlas, chunk.GetTile(localx, localy), worldx, worldy);
        }
    }
}

void AttachPhysical(Entity& e, World* world, Texture2D* atlas, Rectangle spriteRect,
                     float width, float height, Vector2 position) {
    e.AddComponent<MovementComponent>()->position = position;
    e.AddComponent<VisualComponent>(atlas, spriteRect);
    e.AddComponent<ColliderComponent>(world, width * RENDERSCALE, height * RENDERSCALE);
}

void BuildPlayer(Entity& player, World* world, Texture2D* atlas, Vector2 position) {
    AttachPhysical(player, world, atlas, {0, 0, 32, 32}, PLAYERWIDTH, PLAYERHEIGHT, position);

    auto controller = std::make_unique<SelectorNode>();
    controller->AddChild(std::make_unique<ControllerBehavior>());
    player.AddComponent<BehaviorComponent>()->SetRoot(std::move(controller));
    player.AddComponent<WorldComponent>(world, 2);
}

void BuildSlime(Entity& slime, World* world, Texture2D* atlas, Vector2 position) {
    AttachPhysical(slime, world, atlas, {0, 0, SLIME_SPRITE_W, SLIME_SPRITE_H}, SLIMEWIDTH, SLIMEHEIGHT, position);

    auto behavior = std::make_unique<SelectorNode>();
    behavior->AddChild(std::make_unique<WanderBehavior>());
    slime.AddComponent<BehaviorComponent>()->SetRoot(std::move(behavior));
}

int main() {
    InitWindow(GLOBALS::SCREENWIDTH, GLOBALS::SCREENHEIGHT, "Distant: Terra");
    SetTargetFPS(60);
    Image __icon__ = LoadImage(ASSETSPATH "images/icons/__icon__.png");

    SetWindowIcon(__icon__);

    Texture2D TERRAINATLAS = LoadTexture(ASSETSPATH "images/terrain/terrain.png");
    SetTextureFilter(TERRAINATLAS, TEXTURE_FILTER_POINT);

    Texture2D ACTORATLAS = LoadTexture(ASSETSPATH "images/actors/iphrit.png");
    SetTextureFilter(ACTORATLAS, TEXTURE_FILTER_POINT);

    Texture2D SLIMEATLAS = LoadTexture(ASSETSPATH "images/actors/green_slime.png");
    SetTextureFilter(SLIMEATLAS, TEXTURE_FILTER_POINT);

    World world;
    int surfacerow = WORLDTILEHIGH * 0.5;
    GenerateTestTerrain(world, surfacerow);

    auto controllerbehavior = std::make_unique<SelectorNode>();
    controllerbehavior->AddChild(std::make_unique<ControllerBehavior>());
    Entity player;
    Entity g_slime;
    
    BuildPlayer(player, &world, &ACTORATLAS, {800, 200});
    BuildSlime(g_slime, &world, &SLIMEATLAS, {310, 200});

    Camera2D camera{};
    camera.target = player.GetComponent<MovementComponent>()->position;
    camera.offset = { SCREENWIDTH / 2.0f, SCREENHEIGHT / 2.0f };
    camera.zoom = 1.0f;

    while(WindowShouldClose() == false) {
        float delta = GetFrameTime();
        player.Update(delta);
        g_slime.Update(delta);
        camera.target = player.GetComponent<MovementComponent>()->position;

        if (IsKeyPressed(KEY_F1)) DEBUGDRAWCOLLIDERS = !DEBUGDRAWCOLLIDERS;

        BeginDrawing();
        ClearBackground({20, 20, 30, 255});

        BeginMode2D(camera);
        DrawWorld(TERRAINATLAS, world);
        player.Draw();
        g_slime.Draw();
        EndMode2D();

        DrawText("Welcome!", 20, 20, 20, RAYWHITE);
        DrawFPS(10, 50);
        EndDrawing();
    }

    UnloadTexture(ACTORATLAS);
    UnloadTexture(SLIMEATLAS);
    UnloadTexture(TERRAINATLAS);

    UnloadImage(__icon__);

    CloseWindow();
    return 0;
}