#include "tetra.h"
#include "resource_manager.h"
#include "renderer.h"

Tetra::Tetra()
{
	VBOLayout gradient_layout;
	gradient_layout.addLayoutElement(2, GL_FLOAT, GL_FALSE);
	gradient_layout.addLayoutElement(3, GL_FLOAT, GL_FALSE);

	const GLfloat vertexTetra[] = {
		//x(s)  y(t)  r    g     b
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.15f, -0.4f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.15f, -0.4f, 0.0f, 1.0f, 0.0f,
		0.15f, 0.4f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.15f, 0.4f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	VAO tetraVAO;
	VBO tetraVBO;

	tetraVAO.bind();
	tetraVBO.init(vertexTetra, 5 * 9 * sizeof(GLfloat));
	tetraVAO.addBuffer(tetraVBO, gradient_layout, 9);
	tetraVBO.unbind();
	tetraVAO.unbind();

	m_vao = std::move(tetraVAO);
}

void Tetra::render()
{
	ResourceManager* resources = &ResourceManager::getInstance();
	ShaderProgram* mProgram = &resources->getProgram("move");

	mProgram->use();
	mProgram->setUniform("delta", getVecMove());
	Renderer::draw(&m_vao);
	mProgram->unbind();
}

glm::vec3 Tetra::getVecMove()
{
	return glm::vec3(deltaX, deltaY, 0.0f);
}

void Tetra::update(Event e)
{
	if (e.type == EVENT_TYPE::TRANSFORM) {
		auto [ x, y, z] = e.data;
		deltaX += x;
		deltaY += y;
	}
}
