#pragma once
#include "buffer_objects.h"
#include "shader_program.h"

class Renderer {
public:
    static void draw(const VAO& vao, const EBO& ebo);

    static void draw(const VAO& vao);

    static void draw(VAO* vao);

    static void draw(VAO* vao, EBO* ebo);

    static void setClearColor(float r, float g, float b, float a);

    static void clear();

    static void setViewPort(int x, int y, int width, int height);
};
