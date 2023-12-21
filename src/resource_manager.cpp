
#include "resource_manager.h"
#include "game_object.h"

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

	shaderPrograms.emplace("directionalLight", ShaderProgram(readFile("res/shaders/v_lighting.glsl"), readFile("res/shaders/f_lighting.glsl")));

	//Новые модели и текстуры
	m_meshes.emplace("cloud", Mesh("res/meshes/ball.obj"));
	m_textures.emplace("cloud", Texture2D("res/textures/ball.jpg"));
	m_textures.emplace("default", Texture2D("res/textures/default.jpg"));
	m_meshes.emplace("terrain", Mesh("res/meshes/terrain.obj"));
	m_textures.emplace("terrain", Texture2D("res/textures/terrain.jpg"));
	m_meshes.emplace("tree", Mesh("res/meshes/tree.obj"));
	m_textures.emplace("tree", Texture2D("res/textures/tree.jpg"));
	m_meshes.emplace("plane", Mesh("res/meshes/airplane.obj"));
	m_textures.emplace("plane", Texture2D("res/textures/airplane.jpg"));
	m_meshes.emplace("box", Mesh("res/meshes/box.obj"));
	m_textures.emplace("box", Texture2D("res/textures/box.jpg"));
	m_meshes.emplace("lamp", Mesh("res/meshes/lamp.obj"));

	try
	{
		m_textures.emplace("skull", Texture2D("res/textures/skull.jpg"));
		m_textures.emplace("barrel", Texture2D("res/textures/barrel.png"));
	}
	catch (const std::exception& e)
	{
		Logger::error_log(e.what());
	}

	m_meshes.emplace("skull", Mesh("res/meshes/skull.obj"));
	m_meshes.emplace("barrel", Mesh("res/meshes/barrel.obj"));

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
