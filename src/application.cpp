#include "application.h"
#include "logger.hpp"
#include "callback_manager.h"
#include "renderer.h"
#include "game_object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <chrono>
#include <string>
#include <unordered_map>
#define MAX_LIGHTS 10

Application& Application::get_instance()
{
	static Application instance("Window", 900, 900);
	return instance;
}

void Application::init()
{
	if (!glfwInit()) {
		Logger::error_log("Неудалось иницализировать GLFW!");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!window)
	{
		Logger::error_log("Неудалось создать окно!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);

	glfwMakeContextCurrent(window);
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) {
		Logger::error_log("Неудалось инициализровать OpenGL контекст!");
		exit(EXIT_FAILURE);
	}

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	Renderer::setViewPort(0, 0, w, h);

	glfwSwapInterval(1);

	glfwSetErrorCallback(CallbackManager::error_callback);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, CallbackManager::key_callback);
	glfwSetMouseButtonCallback(window, CallbackManager::mouse_button_callback);
	glfwSetCursorPosCallback(window, CallbackManager::cursor_position_callback);
	glfwSetFramebufferSizeCallback(window, CallbackManager::framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	resourceManager = &ResourceManager::getInstance();
	resourceManager->init();
	glEnable(GL_DEPTH_TEST);

	glfwSetCursorPos(window, 450, 450);

}

struct Light {
	enum class Type {
		Point = 0,
		Directional = 1,
		Spot = 2
	};

	int type;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
};

void RenderObject(GameObject* gameObject, ShaderProgram* program);
glm::mat4 RotationMatrix(const glm::vec3& rotationAngles);
void AddLight(Light* source);
void RemoveLight(Light* source);
void RemoveLastLight();
void ApplyLight(ShaderProgram* program, Light* lightSource, int i);

std::unordered_map<std::string, GameObject*> gameObjects;
Light* lightSources[MAX_LIGHTS];
int numLights = 0;

void Application::start()
{

	Renderer::setClearColor(65.0f / 255.0f, 74.0f / 255.0f, 76.0f / 255.0f, 1.0f);

	ResourceManager* resources = &ResourceManager::getInstance();
	Texture2D* texture_skull = &resources->getTexture("skull");
	Texture2D* texture_barrel = &resources->getTexture("barrel");
	Mesh* skull_obj = &resources->getMesh("skull");
	Mesh* barrel_obj = &resources->getMesh("barrel");

#pragma region Materials
	Material defaultMaterial = { glm::vec3(1.0f, 1.0f, 1.0f), // diffuseColor
				 glm::vec3(1.0f, 1.0f, 1.0f),  // specularColor
				 glm::vec3(0.0f, 0.0f, 0.0f),  // emissionColor
				 glm::vec3(0.01f, 0.01f, 0.01f),  // ambientColor
				 32.0f };                      // shininess
	Material planeMaterial = { glm::vec3(1.0f, 1.0f, 1.0f), // diffuseColor
					 glm::vec3(1.0f, 1.0f, 1.0f),  // specularColor
					 glm::vec3(0.0f, 0.0f, 0.0f),  // emissionColor
					 glm::vec3(0.05f, 0.05f, 0.05f),  // ambientColor
					 8.0f };                      // shininess
	Material lampMaterial = { glm::vec3(0.6f, 0.7f, 0.3f), // diffuseColor
					 glm::vec3(1.0f, 1.0f, 1.0f),  // specularColor
					 glm::vec3(0.7f, 0.7f, 0.7f),  // emissionColor
					 glm::vec3(0.01f, 0.01f, 0.01f),  // ambientColor
					 32.0f };                      // shininess
	Material cloudMaterial = { glm::vec3(1.0f, 1.0f, 1.0f), // diffuseColor
					 glm::vec3(0.8f, 0.8f, 1.0f),  // specularColor
					 glm::vec3(0.1f, 0.1f, 0.1f),  // emissionColor
					 glm::vec3(0.3f, 0.3f, 0.3f),  // ambientColor
					 64.0f };
#pragma endregion

#pragma region Meshes and textures
	Texture2D* terrainTex = &resources->getTexture("terrain");
	Texture2D* planeTex = &resources->getTexture("plane");
	Texture2D* treeTex = &resources->getTexture("tree");
	Texture2D* boxTex = &resources->getTexture("box");
	Texture2D* cloudTex = &resources->getTexture("cloud");
	Texture2D* defaultTex = &resources->getTexture("default");
	Mesh* terrainObj = &resources->getMesh("terrain");
	Mesh* planeObj = &resources->getMesh("plane");
	Mesh* treeObj = &resources->getMesh("tree");
	Mesh* boxObj = &resources->getMesh("box");
	Mesh* lampObj = &resources->getMesh("lamp");
	Mesh* cloudObj = &resources->getMesh("cloud");
#pragma endregion

	//GAME OBJECTS
	gameObjects["terrain"] = new GameObject(terrainObj, terrainTex, &defaultMaterial, 4);
	gameObjects["tree"] = new GameObject(treeObj, treeTex, &defaultMaterial, 0.1, glm::vec3(-26.2752, 16.8992, -10.8146), glm::vec3(-90,0,0));
	gameObjects["player"] = new GameObject(planeObj, planeTex, &planeMaterial, 0.01, glm::vec3(-10, 28, -10), glm::vec3(-90, 0, 0));
	gameObjects["lamp0"] = new GameObject(lampObj, defaultTex, &lampMaterial, 0.05, glm::vec3(-20.0657, 17.7, -14.6416));
	gameObjects["lamp1"] = new GameObject(lampObj, defaultTex, &lampMaterial, 0.05, glm::vec3(-49.2922, 14.1686, -42.9111));
	gameObjects["lamp2"] = new GameObject(lampObj, defaultTex, &lampMaterial, 0.05, glm::vec3(14.2697, 16.6533, -8.7417));
	gameObjects["lamp3"] = new GameObject(lampObj, defaultTex, &lampMaterial, 0.05, glm::vec3(-22.5987, 13.2782, 29.4179));

	gameObjects["cloud0"] = new GameObject(cloudObj, cloudTex, &cloudMaterial, 0.75, glm::vec3(-22.5987, 30.2782, -10.8146), glm::vec3(-90, 0, 0));

	//Матрица проекции - не меняется между кадрами, поэтому устанавливается вне цикла
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);

	ShaderProgram* directionalLight = &resources->getProgram("directionalLight");
	directionalLight->use();
	directionalLight->setUniform("projection", projection);
	
	//LIGHTS
	AddLight(new Light{ (int)Light::Type::Directional, 
			glm::vec3(), // pos
			glm::vec3(-0.1, -0.8, -0.3), //dir
			glm::vec3(1), //col
			0.5}); //int
	AddLight(new Light{ (int)Light::Type::Point, 
			glm::vec3(gameObjects["lamp0"]->position),
			glm::vec3(), 
			glm::vec3(1),
			8,
			0.5,
			10});
	AddLight(new Light{ (int)Light::Type::Point,
			glm::vec3(gameObjects["lamp1"]->position),
			glm::vec3(),
			glm::vec3(1),
			8,
			0.5,
			10 });
	AddLight(new Light{ (int)Light::Type::Point,
			glm::vec3(gameObjects["lamp2"]->position),
			glm::vec3(),
			glm::vec3(1),
			8,
			0.5,
			10 });
	AddLight(new Light{ (int)Light::Type::Point,
			glm::vec3(gameObjects["lamp3"]->position),
			glm::vec3(),
			glm::vec3(1),
			8,
			0.5,
			10 });

	directionalLight->setUniform("numLights", numLights);
	for (int i = 0; i < numLights; ++i) {
		ApplyLight(directionalLight, lightSources[i], i);
	}
	directionalLight->unbind();

	// Game loop
	auto start = std::chrono::steady_clock::now();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		Renderer::clear();
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 viewPos = camera.GetPosition();

		directionalLight->use();
		directionalLight->setUniform("view", view);
		directionalLight->setUniform("ViewPos", viewPos);
		directionalLight->unbind();

		for (const auto& x : gameObjects) {
			RenderObject(x.second, directionalLight);
		}

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	resourceManager->destroy();
	glfwTerminate();
}

void Application::close()
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}

Application::~Application()
{
	for (auto& x : primitives)delete(x.second);
}

void Application::select_task(int value)
{
	if (value < 1 || value > 4)return;
	m_current_task = value;
}

void Application::PrintPosition()
{
	auto pos = camera.GetPosition();
	std::cout << "pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}

Application::Application(std::string name, int width, int height) : name(std::move(name)), width(width), height(height) {}

glm::mat4 RotationMatrix(const glm::vec3& rotationAngles) {
	glm::mat4 rotationMatrix = glm::eulerAngleXYZ(
		glm::radians(rotationAngles.x), // pitch (тангаж)
		glm::radians(rotationAngles.y), // yaw (рыскание)
		glm::radians(rotationAngles.z)  // roll (крен)
	);
	return rotationMatrix;
}
void ApplyLight(ShaderProgram* program, Light* lightSource, int i)
{
	std::string prefix = "lights[" + std::to_string(i) + "].";
	program->setUniform((prefix + "type").c_str(), lightSource->type);
	program->setUniform((prefix + "position").c_str(), lightSource->position);
	program->setUniform((prefix + "direction").c_str(), lightSource->direction);
	program->setUniform((prefix + "color").c_str(), lightSource->color);
	program->setUniform((prefix + "intensity").c_str(), lightSource->intensity);
	program->setUniform((prefix + "constant").c_str(), lightSource->constant);
	program->setUniform((prefix + "linear").c_str(), lightSource->linear);
	program->setUniform((prefix + "quadratic").c_str(), lightSource->quadratic);
	program->setUniform((prefix + "cutOff").c_str(), lightSource->cutOff);
}

void AddLight(Light* source) {
	if (numLights < MAX_LIGHTS) {
		lightSources[numLights] = source;
		++numLights;
	}
}

void RemoveLight(Light* source) {
	if (numLights > 0) {
		int indexToRemove = -1;
		for (int i = 0; i < numLights; ++i) {
			if (lightSources[i] == source) {
				indexToRemove = i;
				break;
			}
		}

		if (indexToRemove != -1) {
			lightSources[indexToRemove] = lightSources[numLights - 1];
			--numLights;
		}
	}
}
void RemoveLastLight() {
	if (numLights > 0) {
		--numLights;
	}
}

void RenderObject(GameObject* gameObject, ShaderProgram* program)
{
	//Матрица модели - меняется между кадрами, поэтому устанавливается в цикле
	glm::mat4 model = glm::translate(glm::mat4(1.0f), gameObject->position);
	model *= RotationMatrix(gameObject->rotation);
	model = glm::scale(model, glm::vec3(gameObject->scale));

	program->use();
	program->setUniform("model", model);

	program->setUniform("material.diffuseColor", gameObject->material->diffuseColor);
	program->setUniform("material.specularColor", gameObject->material->specularColor);
	program->setUniform("material.ambientColor", gameObject->material->ambientColor);
	program->setUniform("material.emissionColor", gameObject->material->emissionColor);
	program->setUniform("material.shininess", gameObject->material->shininess);

	glActiveTexture(GL_TEXTURE0);
	gameObject->texture->bind();

	glBindVertexArray(gameObject->mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, gameObject->mesh->vertices.size());
	glBindVertexArray(0);

	gameObject->texture->unbind();
	program->unbind();
}
