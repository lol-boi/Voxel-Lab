#pragma once
#include <glm/glm.hpp>

namespace Config {
    constexpr unsigned SCR_WIDTH = 1500;
    constexpr unsigned SCR_HEIGHT = 900;
    constexpr float FPS_CAP = 60.0f;
    constexpr glm::vec3 INIT_CAM_POS = {320.0f, 255.0f, 320.0f};
    constexpr int RENDER_DISTANCE = 20;
    constexpr int WORLD_SEED = 118282;
}
