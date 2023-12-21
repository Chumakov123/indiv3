#pragma once
#include <string>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "resource_manager.h"
#include "object.h"
#include "camera.h"


class Application {
public:
	enum PlayerMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	static Application& get_instance();

	void init();

	void start();

	void close();

	~Application();

	Application() = delete;

	Application(const Application&) = delete;

	Application& operator=(const Application&) = delete;

	void select_task(int value);

	void PrintPosition();
	void ProcessKeyboard(PlayerMovement direction);


	Camera camera = Camera();
private:
	Application(std::string name, int width, int height);
	int m_current_task = 1;
	std::string name;
	int width;
	int height;
	GLFWwindow* window = nullptr;
	ResourceManager* resourceManager = nullptr;
	std::map<std::string, Renderable*> primitives;
};