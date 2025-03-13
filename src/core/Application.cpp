#include "Application.hpp"
#include "../util/FrameTimer.hpp"
#include "../util/Config.hpp"
#include "InputHandler.hpp"  // Add this include
#include "../external/glfw/include/GLFW/glfw3.h"
#include <memory>
#include <stdexcept>
#include <iostream>
#include "../util/Tick.hpp"


Application::Application()
{
    //Init glfw context before anything else
    initialize();

    //set shader programs
    shader = std::make_unique<Shader>( "../../src/shaders/vs.glsl", "../../src/shaders/fs.glsl");
    shader->use();

    texture_array = std::make_unique<Texture>("../../src/res", true);
    shader->set_int("texture_array", 0);
    std::cout << "HELLO" << std::endl;

    //Init terrain
    terrain = new Terrain(Config::RENDER_DISTANCE, Config::WORLD_SEED);
}

void Application::initialize() {
    // Initialize GLFW and create window
    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        Config::SCR_WIDTH,
        Config::SCR_HEIGHT,
        "Voxel Engine",
        nullptr,
        nullptr
    );

    if(!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Set context and callbacks
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    setup_callbacks();

    // Initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, Config::SCR_WIDTH, Config::SCR_HEIGHT);


    // Initialize camera and terrain
    camera.Position = Config::INIT_CAM_POS;
    //terrain.init_world_chunks(camera.Position);
}

void Application::run() {
    //DebugUI ui;
    Tick game_tick(terrain);
    game_tick.start();
    FrameTimer frame_timer;

    //try {
    //    ui.initialize(window);
    //} catch(const std::exception& e) {
    //    std::cerr << "UI initialization failed: " << e.what() << std::endl;
    //}

    while(!glfwWindowShouldClose(window)) {
        game_tick.updateCameraPosition(camera.Position);
        frame_timer.start_frame();

        // Update phase
        InputHandler::poll(window, camera, frame_timer.delta());

        // Render phase
        render_scene();

        // UI rendering
        //ui.new_frame();
        //ui.render(camera, terrain.render_distance);
        //ui.render_frame();

        // Frame timing
        frame_timer.cap_frame();
        glfwPollEvents();
    }
    game_tick.stop();

    cleanup();
    //ui.shutdown();
}

void Application::render_scene() {

    //BG color
    glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind texture1s
    texture_array->bind(GL_TEXTURE0);


    glm::mat4 model = glm::mat4(1.0f);
    shader->set_mat4("model", model);

    // Set projection matrices
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom),
        (float)Config::SCR_WIDTH / (float)Config::SCR_HEIGHT,
        0.1f,
        1000.0f
    );
    shader->set_mat4("projection", projection);
    shader->set_mat4("view", camera.GetViewMatrix());

    // Render terrain
    terrain->draw();

    // Swap buffers
    glfwSwapBuffers(window);
}

void Application::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::setup_callbacks() {
    auto resize_callback = [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    };

    auto mouse_callback = [](GLFWwindow* window, double xpos, double ypos) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->handle_mouse_input(xpos, ypos);
    };

    auto scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->camera.ProcessMouseScroll(static_cast<float>(yoffset));
    };

    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::handle_mouse_input(double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = Config::SCR_WIDTH / 2.0f;
    static float lastY = Config::SCR_HEIGHT / 2.0f;

    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
