#pragma once
#include <string>
#include <map>
#include "shader_program.h"
#include "buffer_objects.h"
#include "texture.h"
#include "mesh.h"
class ResourceManager {
public:
    static ResourceManager& getInstance();

    ~ResourceManager();

    void init();

    void destroy();

    ShaderProgram& getProgram(const std::string& progName);
    VAO& getVAO(const std::string& vaoName);
    EBO& getEBO(const std::string& vaoName);
    glm::vec3& getColor(const std::string& colorName);
    Texture2D& getTexture(const std::string& textureName);
    Mesh& getMesh(const std::string& meshName);
private:
    ResourceManager();

    ResourceManager(const ResourceManager&) = delete;

    ResourceManager& operator=(const ResourceManager&) = delete;

    ResourceManager& operator=(ResourceManager&& program) = delete;

    ResourceManager(ResourceManager&& program) = delete;

    std::map<std::string, ShaderProgram> shaderPrograms;
    std::map<std::string, VAO> m_vao;
    std::map<std::string, EBO> m_ebo;
    std::map<std::string, glm::vec3> m_colors;
    std::map<std::string, Texture2D> m_textures;
    std::map<std::string, Mesh> m_meshes;
};