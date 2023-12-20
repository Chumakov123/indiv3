#pragma once
#include "object.h"
#include "buffer_objects.h"
#include <array>
class Circle : public Renderable
{
public:
	Circle();

	// Унаследовано через Renderable
	void render() override;

	void update(Event e) override;

private:
	VAO m_vao;
	float x_scale;
	float y_scale;

	float bytify(float color);
	std::array<float, 4> hsv_to_rgb(float hue, float saturation = 100.0, float value = 100.0);  
};

// Вершина
struct Vertex
{
	GLfloat x;
	GLfloat y;
};
