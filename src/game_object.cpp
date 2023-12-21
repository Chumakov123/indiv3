#include "game_object.h"

GameObject::GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material, float s, glm::vec3 p, glm::vec3 r)
{
	mesh = _mesh;
	texture = _texture;
	material = _material;

	position = p;
	rotation = r;

	scale = s;
}

GameObject::GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material, float s, glm::vec3 p)
{
	mesh = _mesh;
	texture = _texture;
	material = _material;

	position = p;
	rotation = glm::vec3();
	scale = s;
}

GameObject::GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material, float s)
{
	mesh = _mesh;
	texture = _texture;
	material = _material;

	position = glm::vec3();
	rotation = glm::vec3();
	scale = s;
}

GameObject::GameObject(Mesh* _mesh, Texture2D* _texture, Material* _material)
{
	mesh = _mesh;
	texture = _texture;
	material = _material;

	position = glm::vec3();
	rotation = glm::vec3(0);
	scale = 1;
}
