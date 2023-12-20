#include <glm/gtc/matrix_transform.hpp>
#include "mesh.h"
#include "texture.h"
#include "material.h"

class GameObject {
public:
	Texture2D* texture;
	Mesh* mesh;
	Material* material;
	glm::vec3 position;
	glm::vec3 rotation;
	float scale;
	GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material, float s, glm::vec3 p, glm::vec3 r);
	GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material, float s, glm::vec3 p);
	GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material, float s);
	GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material);
};