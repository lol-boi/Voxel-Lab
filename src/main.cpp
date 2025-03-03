//#include "core/InputHandler.hpp"
//#include "external/glad/include/glad/glad.h"
//#include "terrain/Terrain.hpp"
//#include <GL/gl.h>
//#include <GL/glext.h>
//#include "external/glfw/include/GLFW/glfw3.h"
//#include "graphics/stb_image.h"
//#include <GLFW/glfw3.h>
//#include <cwchar>
//#include <glm/detail/qualifier.hpp>
//#include <glm/ext/matrix_clip_space.hpp>
//#include <glm/ext/matrix_float4x4.hpp>
//#include <glm/ext/matrix_transform.hpp>
//#include <glm/ext/vector_float3.hpp>
//#include <glm/geometric.hpp>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include "util/Camera.hpp"
//#include "graphics/Shader.hpp"
//#include "graphics/Texture.hpp"
//#include <glm/trigonometric.hpp>
//#include <iostream>
//#include "util/Config.hpp"
//#include "util/FrameTimer.hpp"
//#include "util/Tick.hpp"
//
//
//void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//void handle_mouse_input(double xpos, double ypos);
//void setup_callbacks(GLFWwindow*);
//GLFWwindow* init_glfw();
//void cleanup(GLFWwindow *);
//
////camera
//Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f), -90.0f, 10.0f);
//FrameTimer frame_timer;
//
//
//
//int main(){
//
//    auto window = init_glfw();
//
//    Shader shader_program("../../src/libs/vs.glsl","../../src/libs/fs.glsl");
//    shader_program.use();
//    Texture texture1("../../src/res/awesomeface.png", true);
//    Texture texture2("../../src/res/atlas1.png", true);
//    shader_program.set_int("texture1", 0);
//    shader_program.set_int("texture2", 1);
//
//
//    Terrain terrain = Terrain(12,123);
//    camera.Position = glm::vec3(Config::INIT_CAM_POS);
//    //terrain.init_world_chunks(camera.Position);
//
//    Tick tick(terrain);
//    tick.start();
//
//    while(!glfwWindowShouldClose(window)){
//        tick.updateCameraPosition(camera.Position);
//        //event/input handeling
//        frame_timer.start_frame();
//        InputHandler::poll(window,camera,frame_timer.delta());
//
//        //render_scene__________________________________
//        {
//            glClearColor(0.678f,0.847f,0.902f,0.4f);
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//            texture1.bind(GL_TEXTURE0);
//            texture2.bind(GL_TEXTURE1);
//
//            shader_program.use();
//
//            glm::mat4 model = glm::mat4(1.0f);
//            shader_program.set_mat4("model", model);
//
//            glm::mat4 projection = glm::perspective(
//                    glm::radians(camera.Zoom),
//                    (float)Config::SCR_WIDTH / (float)Config::SCR_HEIGHT,
//                    0.1f,
//                    1000.0f
//                );
//            shader_program.set_mat4("projection", projection);
//
//            shader_program.set_mat4("view", camera.GetViewMatrix());
//
//            terrain.draw();
//            glfwSwapBuffers(window);
//        }
//
//        frame_timer.cap_frame();
//        glfwPollEvents();
//    }
//    tick.stop();
//    //terminate
//    cleanup(window);
//    //tick.join();
//    return 0;
//}
//
//
//
//GLFWwindow* init_glfw(){
//    if(!glfwInit()) {
//            throw std::runtime_error("Failed to initialize GLFW");
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    //window is created using glfw and error handling is done
//    GLFWwindow* window = glfwCreateWindow(
//        Config::SCR_WIDTH ,
//        Config::SCR_HEIGHT,
//        "VoxelLabs", nullptr,
//        nullptr
//    );
//
//    if(!window){
//        std::cout << "ERROR WINDOW NOT INIT" << std::endl;
//        glfwTerminate();
//        return nullptr;
//    }
//
//    glfwMakeContextCurrent(window);
//    //glfwSetWindowUserPointer(window, this);
//    setup_callbacks(window);
//
//    //glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
//    //glfwSetCursorPosCallback(window, mouse_callback);
//    //glfwSetScrollCallback(window, scroll_callback);
//    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }
//    glEnable(GL_DEPTH_TEST);
//    glViewport(0,0, Config::SCR_WIDTH, Config::SCR_HEIGHT);
//
//    camera.Position = Config::INIT_CAM_POS;
//    return window;
//}
//
//
//void cleanup(GLFWwindow* window) {
//    glfwDestroyWindow(window);
//    glfwTerminate();
//}
//
//void setup_callbacks(GLFWwindow* window){
//    auto resize_callback = [](GLFWwindow* window,int width, int height) {
//        glViewport(0, 0, width, height);
//    };
//
//    auto mouse_callback = [](GLFWwindow* window, double xpos, double ypos) {
//        handle_mouse_input(xpos, ypos);
//    };
//
//    auto scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset) {
//        camera.ProcessMouseScroll(static_cast<float>(yoffset));
//    };
//
//    glfwSetFramebufferSizeCallback(window, resize_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//}
//
//void handle_mouse_input(double xpos, double ypos){
//    static bool firstMouse = true;
//    static float lastX = Config::SCR_WIDTH / 2.0f;
//    static float lastY = Config::SCR_HEIGHT / 2.0f;
//
//    if(firstMouse) {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}

#include "core/Application.hpp"
#include <iostream>

int main() {
    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
