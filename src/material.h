#include"mesh.h"

struct Material {
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 emissionColor;
	glm::vec3 ambientColor;
	float shininess;
};