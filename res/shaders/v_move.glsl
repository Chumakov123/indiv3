#version 330 core
layout (location = 0) in vec2 position;// Устанавливаем позицию атрибута в 0
layout (location = 1) in vec3 color;// Устанавливаем цвет

uniform vec3 delta;
out vec3 color_vertex;

void main()
{
    color_vertex = color;
    vec2 newPos = position + vec2(delta);
    gl_Position = vec4(newPos, 0.0, 1.0);// Напрямую передаем vec3 в vec4
}