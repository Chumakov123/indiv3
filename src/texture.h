#pragma once
#include <glad/gl.h>
#include <string>

#include <sub_texture.h>

class Texture2D {
public:

    Texture2D(const char* path);

    ~Texture2D();

    Texture2D() = delete;

    Texture2D(Texture2D&) = delete;

    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D& operator=(Texture2D&& texture2D) noexcept;

    Texture2D(Texture2D&& texture2D) noexcept;

    void bind();

    void unbind();

    int width();

    int height();

    virtual const SubTexture& getSubTexture(const size_t& subTexName);

public:
    int mWidth = 0;
    int mHeight = 0;
    int channel = 0;
    GLenum format;
    GLuint textureID = 0;


};
