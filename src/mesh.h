#pragma once
#include "texture.h"
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


std::vector<std::string> split(const std::string& s, const char delimiter);

struct MeshVertex {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texture[2];
};

class Mesh{
private:
    void parseFile(const std::string& filePath);
    void InitPositionBuffers();
public:
    std::vector<MeshVertex> vertices;
    GLuint VBO;
    GLuint VAO;
    Mesh(const char* meshPath);

    ~Mesh();

    Mesh() = delete;
    Mesh(Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& mesh) noexcept;
    Mesh(Mesh&& mesh) noexcept;
};