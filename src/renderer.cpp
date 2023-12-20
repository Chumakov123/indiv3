#include "renderer.h"

void Renderer::draw(const VAO& vao, const EBO& ebo) {

    vao.bind();
    ebo.bind();
    glDrawElements(GL_TRIANGLES, ebo.count(), GL_UNSIGNED_INT, 0);
    vao.unbind();
    ebo.unbind();
}

void Renderer::draw(const VAO& vao) {
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, vao.count());
    vao.unbind();
}

void Renderer::draw(VAO* vao) {
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, vao->count());
    vao->unbind();
}

void Renderer::draw(VAO* vao, EBO* ebo)
{
    vao->bind();
    ebo->bind();
    glDrawElements(GL_TRIANGLES, ebo->count(), GL_UNSIGNED_INT, 0);
    vao->unbind();
    ebo->unbind();
}

void Renderer::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewPort(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}