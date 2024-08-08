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
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window, float input_debug);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int compile_shader(unsigned int, const std::string&);
static unsigned int create_shader(const std::string&, const std::string&);
GLFWwindow* init_glfw();
unsigned int set_texture(const char * path, float rgba);
void set_polygon_mode();

const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 900;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//camera
Camera camera(glm::vec3(80.0f, 80.0f, 80.0f), glm::vec3(0.0f,1.0f,0.0f), -90.0f, 10.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT/ 2.0f;
bool firstMouse = true;


int main(){

    auto window = init_glfw();

    Shader shader_program("../../src/libs/vs.txt","../../src/libs/fs.txt");
    const char*  texture_path1 = "../../src/res/block.png";
    const char* texture_path2 = "../../src/res/awesomeface.png";
    //const char* texture_path3 = "../../src/res/up.png";
    unsigned int texture1, texture2;
    texture1 = set_texture(texture_path1,true);
    texture2 = set_texture(texture_path2,true);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shader_program.use();
    shader_program.set_int("texture1", 0);
    shader_program.set_int("texture2", 1);

    Terrain t(16,8);
    t.gen_terrain(1.0f,2.0f,4.0f,  1.0f,0.5f,0.25f);
    t.init_world_chunks();



    while(!glfwWindowShouldClose(window)){
        //event/input handeling
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window,false);

        //rendering__________________________________

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
        projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        unsigned int projcetion_loc = glGetUniformLocation(shader_program.ID, "projection");
        glUniformMatrix4fv(projcetion_loc, 1, GL_FALSE, &projection[0][0]);


        glm::mat4 view = camera.GetViewMatrix();
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        unsigned int view_loc = glGetUniformLocation(shader_program.ID, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);

        t.draw_terrain();

       // swaps the buffer swaps the colour buffer(a buffer which contais the
       // colour val of each rendering pixel) and show it to the screen
       glfwSwapBuffers(window);
       // checks for envent triggering
       glfwPollEvents();
    }
    //deallocation of buffers

    //terminate
    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow *window, float input_dbug){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
        if(input_dbug)
            std::cout << "W" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        if(input_dbug)
            std::cout << "S" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
        if(input_dbug)
            std::cout << "A" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
        if(input_dbug)
            std::cout << "D" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera.ProcessKeyboard(UP,deltaTime);
        if(input_dbug)
            std::cout << "SPACE" << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ){
        camera.ProcessKeyboard(DOWN,deltaTime);
        if(input_dbug)
            std::cout << "SHIFT" << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_P) ==  GLFW_PRESS){
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
