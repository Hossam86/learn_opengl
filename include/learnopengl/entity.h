#pragma once
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include <list>
#include <memory>

#include <learnopengl/model.h>

#include <glm/glm.hpp>

struct Transform
{
	// space information
	// local space information
	glm::vec3 m_pos = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_eular_rot = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_scale = {1.0f, 1.0, 1.0f};

	// Global space information concatenate in matrix
	glm::mat4 model_matrix = glm::mat4(1.0f);

	// Dirty flag
	bool is_dirty = true;

protected:
	glm::mat4
	getLocalModelMatrix()
	{
		// get rotation matrix from euler angles
		glm::mat4 transform_x = glm::rotate(glm::mat4(1.0), glm::radians(m_eular_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 transform_y = glm::rotate(glm::mat4(1.0), glm::radians(m_eular_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 transform_z = glm::rotate(glm::mat4(1.0), glm::radians(m_eular_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

		// Y * X * Z
		glm::mat4 rotation_matrix = transform_y * transform_x * transform_z;

		// translation matrix
		glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0), m_pos);

		glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0), m_scale);

		// translation * rotation * scale (also know as TRS matrix)
		return (translate_matrix * rotation_matrix * scale_matrix);
	}
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