#pragma once
#include "buffer_objects.h"
#include "object.h"
#include <glm/vec3.hpp>

class Tetra: public Renderable {
public:
	Tetra();
	// Унаследовано через Renderable
	virtual void render() override;
	// Унаследовано через Renderable
	void update(Event e) override;

private:
	float deltaX = 0.0f;
	float deltaY = 0.0f;
	glm::vec3 getVecMove();
	VAO m_vao;
	EBO m_ebo;
};



