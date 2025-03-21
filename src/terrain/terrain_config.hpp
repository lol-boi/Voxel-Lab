#pragma once
#include "FastNoiseLite.h"
#include "../util/Config.hpp"

namespace Terrain_Config {
    const static FastNoiseLite gen_e(Config::WORLD_SEED);
    const static FastNoiseLite gen_m(Config::WORLD_SEED);
    constexpr float Elevation1 = 1.0f;
    constexpr float Elevation2 = 0.85f;
    constexpr float Elevation3 = 0.51f;
    constexpr float Elevation4 = 0.15f;
    constexpr float Elevation5 = 0.07f;
    constexpr float Elevation6 = 0.03f;

    constexpr float Exp = 5.0f;

    constexpr float Moisture1= 1.0f;
    constexpr float Moisture2= 0.66f;
    constexpr float Moisture3= 0.29f;
    constexpr float Moisture4= 0.73f;
    constexpr float Moisture5= 0.14f;
    constexpr float Moisture6= 0.67f;
}
