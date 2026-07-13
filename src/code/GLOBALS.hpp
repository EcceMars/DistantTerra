#pragma once

namespace GLOBALS {
    constexpr int TILESIZE = 16;

    constexpr int SCREENWIDTH = 1280;
    constexpr int SCREENHEIGHT = 720;

    constexpr int RENDERSCALE = 2;

    constexpr int WORLDTILEWIDE = SCREENWIDTH / (TILESIZE * RENDERSCALE);
    constexpr int WORLDTILEHIGH = SCREENHEIGHT / (TILESIZE * RENDERSCALE);

    constexpr float DELTALIMIT = 1.0f / 60.0f;

    constexpr float SPRITEUNIVERSALYOFFSET = -8.0f;

    constexpr float GRAVITYBASE = 900.0f;
    constexpr float JUMPVELOCITY = -420.0f;
    constexpr float MOVESPEED = 200.0f;
    constexpr float FRICTION = 800.0f;

    constexpr float PLAYERWIDTH = 18;
    constexpr float PLAYERHEIGHT = 24;

    constexpr float SLIME_SPRITE_W = 16;
    constexpr float SLIME_SPRITE_H = 16;
    constexpr float SLIMEWIDTH = 12;
    constexpr float SLIMEHEIGHT = 8;

    inline bool DEBUGDRAWCOLLIDERS = true;
}