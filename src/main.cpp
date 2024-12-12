#include "libs/glad/include/glad/glad.h"
#include "libs/Terrain.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include "libs/glfw/include/GLFW/glfw3.h"
#include "libs/stb_image.h"
#include <cwchar>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "libs/camera.h"
#include "libs/shader.hpp"
#include <glm/trigonometric.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <functional>
#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_impl_glfw.h"
#include "libs/imgui/imgui_impl_opengl3.h"
#include <stdio.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window, float input_debug);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int compile_shader(unsigned int, const std::string&);
static unsigned int create_shader(const std::string&, const std::string&);
void thread_function(Terrain &);
GLFWwindow* init_glfw();
unsigned int set_texture(const char * path, float rgba);
void set_polygon_mode();


const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 900;

const float FPS_CAP = 60.0f;
const float FRAME_DURATION = 1.0f / FPS_CAP;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool mouseDetached = false;

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f), -90.0f, 10.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT/ 2.0f;
bool firstMouse = true;


int main(){

    auto window = init_glfw();

    Shader shader_program("../../src/libs/vs.glsl","../../src/libs/fs.glsl");
    const char*  texture_path1 = "../../src/res/awesomeface.png";
    const char* texture_path2 = "../../src/res/atlas1.png";
    //const char* texture_path3 = "../../src/res/up.png";
    unsigned int texture1, texture2;
    texture1 = set_texture(texture_path1,true);
    texture2 = set_texture(texture_path2,true);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shader_program.use();
    shader_program.set_int("texture1", 0);
    shader_program.set_int("texture2", 1);


    Terrain terrain = Terrain(12,123);
    camera.Position = glm::vec3(32*10,255,32*10);

    terrain.init_world_chunks(camera.Position);
    terrain.upload_buffers();

    std::thread tick(thread_function,std::ref(terrain));
    tick.detach();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

   // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    while(!glfwWindowShouldClose(window)){
        //event/input handeling
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window,false);

        //terrain.init_world_chunks(camera.Position);

        //rendering__________________________________

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Debug Window");                          // Create a window called "Hello, world!" and append into it.
            glm::vec3 pos = camera.Position;
            ImGui::Text("Current chunk positon: [%.0f, %.f, %.f]", pos.x/32, pos.y/32, pos.z/32);            // Display some text (you can use a format strings too)
            ImGui::Text("Current camera positon: [%.1f, %.1f, %.1f]",camera.Position.x, camera.Position.y, camera.Position.z);               // Display some text (you can use a format strings too)
            ImGui::Text("Current rendered chunks: %d ",terrain.render_distance);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        glClearColor(0.678f,0.847f,0.902f,0.4f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader_program.use();

        glm::mat4 model = glm::mat4(1.0f);
        unsigned int model_loc = glGetUniformLocation(shader_program.ID, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 256.0f);
        unsigned int projcetion_loc = glGetUniformLocation(shader_program.ID, "projection");
        glUniformMatrix4fv(projcetion_loc, 1, GL_FALSE, &projection[0][0]);


        glm::mat4 view = camera.GetViewMatrix();
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
        unsigned int view_loc = glGetUniformLocation(shader_program.ID, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);


        if(terrain.is_buffer_updated == true){
            terrain.upload_buffers();
        }
        terrain.draw_terrain();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        float frameTime = static_cast<float>(glfwGetTime()) - currentFrame;
        if (frameTime < FRAME_DURATION) {
            std::this_thread::sleep_for(std::chrono::duration<float>(FRAME_DURATION - frameTime));
        }
    }
    //deallocation of buffers

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    //terminate

    glfwDestroyWindow(window);
    glfwTerminate();
    tick.join();
    return 0;
}

void process_input(GLFWwindow *window, float input_dbug) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        if (input_dbug)
            std::cout << "W" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (input_dbug)
            std::cout << "S" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        if (input_dbug)
            std::cout << "A" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
        if (input_dbug)
            std::cout << "D" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
        if (input_dbug)
            std::cout << "SPACE" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
        if (input_dbug)
            std::cout << "SHIFT" << std::endl;
    }

    // Toggle mouse attachment/detachment with the M key
    static bool mKeyWasPressed = false;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!mKeyWasPressed) {  // If 'M' was just pressed
            mouseDetached = !mouseDetached;  // Toggle the state
            if (mouseDetached) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // Detach mouse
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Capture mouse
            }
        }
        mKeyWasPressed = true;
    } else {
        mKeyWasPressed = false;
    }


    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        set_polygon_mode();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
       glViewport(0,0,width,height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
GLFWwindow* init_glfw(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //window is created using glfw and error handling is done
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT, "VoxelLabs", NULL, NULL);
    if(window == NULL){
        std::cout << "ERROR WINDOW NOT INIT" << std::endl;
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    return window;
}
unsigned int set_texture(const char * path, float rgba){
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nr_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height,  &nr_channels, 0);
    if(data){
        if(rgba == true)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Error in loading texture " << path << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

void set_polygon_mode(){
    static bool state = 1;
    state = !state;
    if(state){
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
}

void thread_function(Terrain &t) {
    const std::chrono::milliseconds interval(50); // Time interval (50ms)
    while (true) {
        auto start = std::chrono::steady_clock::now();
        bool state = t.init_world_chunks(camera.Position);

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (elapsed < interval) {
            std::this_thread::sleep_for(interval - elapsed);
        }
    }
}
