#include "callback_manager.h"
#include <iostream>
#include <application.h>
void CallbackManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, nowHeight;
		glfwGetWindowSize(window, &width, &nowHeight);

		std::cout << "click - x: " << xpos << " y: " << nowHeight - ypos << std::endl;
	}
}

void CallbackManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		Application::get_instance().PrintPosition();
	}
	if (key == GLFW_KEY_W) {
		Application::get_instance().camera.ProcessKeyboard(FORWARD, 0.2);
	}
	if (key == GLFW_KEY_D) {
		Application::get_instance().camera.ProcessKeyboard(RIGHT, 0.2);
	}
	if (key == GLFW_KEY_S) {
		Application::get_instance().camera.ProcessKeyboard(BACKWARD, 0.2);
	}
	if (key == GLFW_KEY_A) {
		Application::get_instance().camera.ProcessKeyboard(LEFT, 0.2);
	}
	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3 || key == GLFW_KEY_4) && action == GLFW_PRESS) {
		std::cout << "click - " << key - 48 << std::endl;
		Application::get_instance().select_task(key - 48);
	}

}

void CallbackManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	int width, nowHeight;
	glfwGetWindowSize(window, &width, &nowHeight);
	Application::get_instance().camera.ProcessMouseMovement(xpos, ypos);

}
void CallbackManager::error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

void CallbackManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}