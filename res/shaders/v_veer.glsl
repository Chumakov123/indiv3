#version 330 core
layout (location = 0) in vec2 position;// Устанавливаем позицию 
layout (location = 1) in vec3 color;// Устанавливаем цвет

out vec3 color_vertex;

void main()
{
    color_vertex = color;
    gl_Position = vec4(position, 0.0, 1.0);// Напрямую передаем vec3 в vec4
}