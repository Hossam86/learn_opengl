#ifndef ENTITY_H
#define ENTITY_H
#include <list>
#include <memory>

#include <learnopengl/model.h>

#include <glm/glm.hpp>

struct Transform
{
	// space information
	// local space information
	glm::vec3 pos = {0.0f, 0.0f, 0.0f};
	glm::vec3 eular_rot = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale = {1.0f, 1.0, 1.0f};

	// Global space information concatenate in matrix
	glm::mat4 model_matrix = glm::mat4(1.0f);
};

class Entity
{
public:
	// Scene graph
	std::list<unique_ptr<Entity>> children;
	Entity* parent;

	// Space Transform
	Transform transform;

	// underlying model
	Model* pModel;

	Entity(Model& model) : pModel(&model){};

	// Add child. Argument input is argument of any constructor that you create. By default you can use the default
	// constructor and don't put argument input.
	template<typename... Targs>
	void
	add_child(Targs&... args)
	{
		children.emplace_back(std::make_unique<Entity>(args...));
		children.back()->parent = this;
	}
};
#endif