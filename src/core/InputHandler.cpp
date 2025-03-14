#include "InputHandler.hpp"
#include "../external/glfw/include/GLFW/glfw3.h"
#include "../util/Camera.hpp"

bool InputHandler::mouse_detached = false;
bool InputHandler::render_toggle = true;
bool InputHandler::polygon_mode_state = true;

void InputHandler::poll(GLFWwindow* window, Camera& camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

    // Toggle mouse attachment/detachment with the M key
    static bool mKeyWasPressed = false;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!mKeyWasPressed) {  // If 'M' was just pressed
            mouse_detached = !mouse_detached;  // Toggle the state
            if (mouse_detached) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // Detach mouse
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Capture mouse
            }
        }
        mKeyWasPressed = true;
    } else {
        mKeyWasPressed = false;
    }
    {
        static bool p_key_pressed = false;
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !p_key_pressed) {
            p_key_pressed = true;
            set_polygon_mode();
        }
        if (p_key_pressed && glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
            p_key_pressed = false;
        }
    }
    {
        static bool r_key_pressed = false;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS ) {
            if (!r_key_pressed) {
                render_toggle = !render_toggle;
                r_key_pressed = true;
            }
        } else{
            r_key_pressed = false;
        }
    }
}


void InputHandler::set_polygon_mode(){
    if(polygon_mode_state){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    polygon_mode_state = !polygon_mode_state;
}
