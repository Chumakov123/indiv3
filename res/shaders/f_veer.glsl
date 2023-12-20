#version 330 core

in vec3 color_vertex;
out vec4 color;

void main()
{
    color = vec4(color_vertex, 1.0f);
}