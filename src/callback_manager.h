#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
class CallbackManager {
public:
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    static void error_callback(int error, const char* description);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    
    CallbackManager() = delete;

    ~CallbackManager() = delete;
};
