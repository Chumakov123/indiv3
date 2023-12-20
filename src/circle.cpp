#include "circle.h"
#include "resource_manager.h"
#include "renderer.h"


Circle::Circle() : x_scale(1.f), y_scale(1.f)
{
	const int circleVertexCount = 360;
	const float deg2rad = 3.1415926535 / 180.0;

	VAO vao;
	VBO vbo_pos;
	VBO vbo_color;

	VBOLayout pos_layout;
	pos_layout.addLayoutElement(2, GL_FLOAT, GL_FALSE);
	VBOLayout color_layout;
	color_layout.addLayoutElement(4, GL_FLOAT, GL_FALSE);
	// Цвет треугольника
	std::array<std::array<float, 4>, circleVertexCount * 3> colors = {};

	Vertex circle[circleVertexCount * 3] = {};
	for (int i = 0; i < circleVertexCount; i++) {
		circle[i * 3] = { 0.5f * (float)cos(i * (360.0 / circleVertexCount) * deg2rad),
			0.5f * (float)sin(i * (360.0 / circleVertexCount) * deg2rad) };
		circle[i * 3 + 1] = { 0.5f * (float)cos((i + 1) * (360.0 / circleVertexCount) * deg2rad),
			0.5f * (float)sin((i + 1) * (360.0 / circleVertexCount) * deg2rad) };
		circle[i * 3 + 2] = { 0.0f, 0.0f };
		colors[i * 3] = hsv_to_rgb(i % 360);
		colors[i * 3 + 1] = hsv_to_rgb((i + 1) % 360);
		colors[i * 3 + 2] = { 1.0, 1.0, 1.0, 1.0 };
	}

	vao.bind();
	vbo_pos.init(circle, sizeof(circle));
	vbo_color.init(colors.data(), sizeof(colors));
	vao.addBuffer(vbo_pos, pos_layout, circleVertexCount * 3);
	vao.addBuffer(vbo_color, color_layout, circleVertexCount * 3);
	vbo_color.unbind();
	vbo_pos.unbind();
	vao.unbind();

	m_vao = std::move(vao);
}

void Circle::render()
{
	ResourceManager* resources = &ResourceManager::getInstance();
	ShaderProgram* mProgram = &resources->getProgram("circle");

	mProgram->use();
	mProgram->setUniform("x_scale", x_scale);
	mProgram->setUniform("y_scale", y_scale);
	Renderer::draw(&m_vao);
	mProgram->unbind();
}

void Circle::update(Event e)
{
	if (e.type == EVENT_TYPE::SCALE) {
		auto [x, y, z] = e.data;
		x_scale += x;
		y_scale += y;
	}
}

float  Circle::bytify(float color)
{
	return (1 / 100.0) * color;
}

std::array<float, 4>  Circle::hsv_to_rgb(float hue, float saturation, float value)
{
	int sw = (int)floor(hue / 60) % 6;
	float vmin = ((100.0f - saturation) * value) / 100.0;
	float a = (value - vmin) * (((int)hue % 60) / 60.0);
	float vinc = vmin + a;
	float vdec = value - a;
	switch (sw)
	{
	case 0:
		return { bytify(value), bytify(vinc), bytify(vmin), 1.0 };
	case 1:
		return { bytify(vdec), bytify(value), bytify(vmin), 1.0 };
	case 2:
		return { bytify(vmin), bytify(value), bytify(vinc), 1.0 };
	case 3:
		return { bytify(vmin), bytify(vdec), bytify(value), 1.0 };
	case 4:
		return { bytify(vinc), bytify(vmin), bytify(value), 1.0 };
	case 5:
		return { bytify(value), bytify(vmin), bytify(vdec), 1.0 };
	}
	return { 0, 0, 0 , 0 };
}
