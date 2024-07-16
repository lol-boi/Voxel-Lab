#include "libs/glad/include/glad/glad.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
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


const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 900;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT/ 2.0f;
bool firstMouse = true;


int main(){

    auto window = init_glfw();


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f,0.0f,1.0f)
    //    glm::vec3( 2.0f, 5.0f, -15.0f),
    //    glm::vec3(-1.5f, -2.2f, -2.5f),
    //    glm::vec3(-3.8f, -2.0f, -12.3f),
    //    glm::vec3( 2.4f, -0.4f, -3.5f),
    //    glm::vec3(-1.7f, 3.0f, -7.5f),
    //    glm::vec3( 1.3f, -2.0f, -2.5f),
    //    glm::vec3( 1.5f, 2.0f, -2.5f),
    //    glm::vec3( 1.5f, 0.2f, -1.5f),
    //    glm::vec3(-1.3f, 1.0f, -1.5f)
    };



    //    unsigned int indices[] = { 0,1,3,
    //                            1,2,3};




    unsigned int element_buffer_obj, vertex_array_object, vertex_buffer_object;
    glGenVertexArrays(1,&vertex_array_object);
    glGenBuffers(1,&vertex_buffer_object);
    glBindVertexArray(vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 5, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float) * 5, (void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,0);


   Shader shader_program("../src/libs/vs.txt","../src/libs/fs.txt");
   const char*  texture_path1 = "../src/res/container.jpg";
   const char* texture_path2 = "../src/res/awesomeface.png";
   unsigned int texture1, texture2;
   texture1 = set_texture(texture_path1,false);
   texture2 = set_texture(texture_path2,true);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   shader_program.use();
   shader_program.set_int("texture1", 0);
   shader_program.set_int("texture2", 1);


    while(!glfwWindowShouldClose(window)){
        //event/input handeling
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window,false);

       //rendering__________________________________
       //colour to clear the screen with black by default
       glClearColor(0.2f,0.3f,0.3f,0.1f);
       //buffer which should be cleared from the screen at the start of each loop
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //       glUseProgram(shader_program);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, texture1);
       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D, texture2);

       shader_program.use();

       glm::mat4 projection = glm::mat4(1.0f);
       projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
       unsigned int projcetion = glGetUniformLocation(shader_program.ID, "projection");
       //        glm::mat4 model  = glm::mat4(1.0f);
       glm::mat4 view = camera.GetViewMatrix();
       view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
       unsigned int view_loc = glGetUniformLocation(shader_program.ID, "view");


       glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
       glUniformMatrix4fv(projcetion, 1, GL_FALSE, &projection[0][0]);
       glBindVertexArray(vertex_array_object);


       for(float x = 0; x<1; x++){
           for(float z = 0; z<1; z++){
               for(int y = 0; y<1; y++){
                   glm::vec3 cube_position = glm::vec3(x,y,z);
                   glm::mat4 model = glm::mat4(1.0f);
                   model = glm::translate(model, cube_position);
                   //float angle = glfwGetTime() * 25.0f;
                   //model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
                   unsigned int model_loc = glGetUniformLocation(shader_program.ID, "model");
                   glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
                   glDrawArrays(GL_TRIANGLES, 0, 36);
               }
           }

       }
        //shader_program.setMat4("projection", projection);
        //color changing shit ____________________________________________________
       //double time_val = glfwGetTime();
       //float green_val = (sin(time_val)/2.0f + 0.5f);
       //int vertex_color_location = glGetUniformLocation(shader_program,"our_color");
       //glUniform4f(vertex_color_location,0.0f,green_val, 0.0f, 1.0f);

       //glDrawArrays(GL_TRIANGLES,0,3);//This uses the currently bound vao and uses its data
       glBindVertexArray(vertex_array_object);
       glDrawArrays(GL_TRIANGLES,0,36);
       //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       // swaps the buffer swaps the colour buffer(a buffer which contais the
       // colour val of each rendering pixel) and show it to the screen
       glfwSwapBuffers(window);
       // checks for envent triggering
       glfwPollEvents();
    }
    //deallocation of buffers
    glDeleteVertexArrays(1,&vertex_array_object);
    glDeleteBuffers(1,&vertex_buffer_object);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT, "Demo OpenGL Title", NULL, NULL);
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
    return window;
}
unsigned int set_texture(const char * path, float rgba){
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
