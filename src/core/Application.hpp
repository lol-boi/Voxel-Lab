#pragma once
#include "../terrain/Terrain.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include "../graphics/Shader.hpp"
#include "../util/Camera.hpp"
#include "../graphics/Texture.hpp"

class Application {
public:
    Application();
    void run();

private:
    void render_scene();
    void setup_callbacks();
    void handle_mouse_input(double xpos, double ypos);
    void initialize();
    void main_loop();
    void cleanup();

    GLFWwindow* window;
    std::unique_ptr<Shader> shader;
    Terrain* terrain;
    Camera camera;
    std::unique_ptr<Texture> texture1;
    std::unique_ptr<Texture> texture2;
};
