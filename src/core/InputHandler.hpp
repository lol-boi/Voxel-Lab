#pragma once
#include "../util/Camera.hpp"
#include <GLFW/glfw3.h>

class InputHandler {
    public:
        static void poll(GLFWwindow* window, Camera& camera, float deltaTime);
    private:
        static bool mouse_detached;
        static bool render_toggle;
};
