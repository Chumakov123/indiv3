
#include "resource_manager.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <chrono>
#include "logger.hpp"
static std::string readFile(const std::string& path) {
	std::ifstream input_file(path);
	if (!input_file.is_open()) {
		std::cerr << "Could not open the file - '"
			<< path << "'" << std::endl;
		exit(EXIT_FAILURE);
	}
	return std::string{ (std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>() };
}

// Функция для генерации случайного числа в диапазоне [min, max)
float randomFloat(float min, float max) {
	static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static std::mt19937 rng(static_cast<unsigned>(seed));
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(rng);
}

ResourceManager::ResourceManager() {
	std::cout << "Constructor ResourceManager (" << this << ") called " << std::endl;
	m_colors["randomColor"] = glm::vec3(randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f));
	m_colors["default"] = glm::vec3(0.0f, 1.0f, 0.0f);
}

ResourceManager::~ResourceManager() {
	std::cout << "Destructor ResourceManager (" << this << ") called " << std::endl;
}

void ResourceManager::init() {

	shaderPrograms.emplace("default", ShaderProgram(readFile("res/shaders/v_default.glsl"), readFile("res/shaders/f_default.glsl")));
	shaderPrograms.emplace("custom", ShaderProgram(readFile("res/shaders/v_default.glsl"), readFile("res/shaders/f_custom_color.glsl")));
	shaderPrograms.emplace("gradient", ShaderProgram(readFile("res/shaders/v_veer.glsl"), readFile("res/shaders/f_veer.glsl")));
	shaderPrograms.emplace("texture", ShaderProgram(readFile("res/shaders/v_texture.glsl"), readFile("res/shaders/f_texture.glsl")));
	shaderPrograms.emplace("circle", ShaderProgram(readFile("res/shaders/v_circle.glsl"), readFile("res/shaders/f_circle.glsl")));
	shaderPrograms.emplace("move", ShaderProgram(readFile("res/shaders/v_move.glsl"), readFile("res/shaders/f_veer.glsl")));
	shaderPrograms.emplace("model", ShaderProgram(readFile("res/shaders/v_model.glsl"), readFile("res/shaders/f_model.glsl")));
	shaderPrograms.emplace("directionalLight", ShaderProgram(readFile("res/shaders/v_light.glsl"), readFile("res/shaders/f_directional_light.glsl")));

	try
	{
		m_textures.emplace("skull", Texture2D("res/textures/skull.jpg"));
		m_textures.emplace("default", Texture2D("res/textures/awesomeface.png"));
		m_textures.emplace("container", Texture2D("res/textures/container.jpg"));
		m_textures.emplace("barrel", Texture2D("res/textures/barrel.png"));
		//m_textures.emplace("drum", Texture2D("res/textures/drum.png"));
	}
	catch (const std::exception& e)
	{
		Logger::error_log(e.what());
	}

	m_meshes.emplace("skull", Mesh("res/meshes/skull.obj"));
	m_meshes.emplace("barrel", Mesh("res/meshes/barrel.obj"));
	//m_meshes.emplace("drum", Mesh("res/meshes/drum.obj"));

	VBOLayout menuVBOLayout;
	menuVBOLayout.addLayoutElement(2, GL_FLOAT, GL_FALSE);

	const GLfloat vertex[] = {
		//x(s)  y(t)
		0.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f
	};
	VAO baseVAO;
	VBO baseVBO;

	baseVAO.bind();
	baseVBO.init(vertex, 2 * 3 * sizeof(GLfloat));
	baseVAO.addBuffer(baseVBO, menuVBOLayout, 3);
	baseVBO.unbind();
	baseVAO.unbind();

	m_vao.emplace("default", std::move(baseVAO));
}

void ResourceManager::destroy() {
	//std::cout << "Destructor ResourceManager (" << this << ") called " << std::endl;
	shaderPrograms.clear();
	m_colors.clear();
	m_vao.clear();
	m_ebo.clear();
	m_textures.clear();
}


ShaderProgram& ResourceManager::getProgram(const std::string& progName) {
	//return shaderPrograms[progName];
	auto it = shaderPrograms.find(progName);
	if (it != shaderPrograms.end()) {
		return it->second;
	}
	return shaderPrograms.find("default")->second;
}

VAO& ResourceManager::getVAO(const std::string& vaoName)
{
	auto it = m_vao.find(vaoName);
	if (it != m_vao.end()) {
		return it->second;
	}
	return m_vao.find("default")->second;
}

EBO& ResourceManager::getEBO(const std::string& vaoName)
{
	auto it = m_ebo.find(vaoName);
	if (it != m_ebo.end()) {
		return it->second;
	}
	return m_ebo.find("veer")->second;
}

glm::vec3& ResourceManager::getColor(const std::string& colorName)
{
	auto it = m_colors.find(colorName);
	if (it != m_colors.end()) {
		return it->second;
	}
	return m_colors.find("default")->second;
}

Texture2D& ResourceManager::getTexture(const std::string& textureName)
{
	auto it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		return it->second;
	}
	return m_textures.find("default")->second;

}

Mesh& ResourceManager::getMesh(const std::string& meshName)
{
	auto it = m_meshes.find(meshName);
	if (it != m_meshes.end()) {
		return it->second;
	}
	return m_meshes.find("default")->second;
}

ResourceManager& ResourceManager::getInstance() {
	static ResourceManager instance;

	return instance;
}
