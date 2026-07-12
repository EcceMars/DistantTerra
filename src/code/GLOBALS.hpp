#pragma once

namespace GLOBALS {
    constexpr int TILESIZE = 16;

    constexpr int SCREENWIDTH = 1280;
    constexpr int SCREENHEIGHT = 720;

    constexpr int RENDERSCALE = 2;

    constexpr int WORLDTILEWIDE = SCREENWIDTH / (TILESIZE * RENDERSCALE);
    constexpr int WORLDTILEHIGH = SCREENHEIGHT / (TILESIZE * RENDERSCALE);

    constexpr float GRAVITYBASE = 900.0f;
    constexpr float JUMPVELOCITY = -420.0f;
    constexpr float MOVESPEED = 200.0f;
    constexpr float FRICTION = 800.0f;

    constexpr float PLAYERWIDTH = 24;
    constexpr float PLAYERHEIGHT = 24;
}