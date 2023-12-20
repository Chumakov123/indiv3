#include <sub_texture.h>

SubTexture::SubTexture(const glm::vec2& leftBottom, const glm::vec2& rightTop):left_bottom(leftBottom),right_top(rightTop){}

SubTexture::SubTexture():left_bottom(0.0f), right_top(1.0f) {}
