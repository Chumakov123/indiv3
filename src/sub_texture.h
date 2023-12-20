#pragma once

#include <glm/vec2.hpp>

class SubTexture {
public:

    SubTexture(const glm::vec2& leftBottom, const glm::vec2& rightTop);

    SubTexture();

    glm::vec2 left_bottom;
    glm::vec2 right_top;

};