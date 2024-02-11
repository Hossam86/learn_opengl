#pragma once
#include <list>
#include <memory>

#include <glm/glm.hpp>
#include <learnopengl/model.h>

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
	get_local_model_matrix()
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

public:
	void
	compute_model_matrix()
	{
		model_matrix = get_local_model_matrix();
		is_dirty = false;
	}

	void
	compute_model_matrix(const glm::mat4& parent_model_matrix)
	{
		model_matrix = parent_model_matrix * get_local_model_matrix();
		is_dirty = false;
	}

	void
	set_local_position(const glm::vec3& pos)
	{
		m_pos = pos;
		is_dirty = true;
	}

	void
	set_local_rotation(const glm::vec3& rot)
	{
		m_eular_rot = rot;
		is_dirty = true;
	}
	void
	set_local_scale(const glm::vec3& scale)
	{
		m_scale = scale;
		is_dirty = true;
	}

	const glm::vec3&
	get_global_position() const
	{
		return model_matrix[3];
	}

	const glm::vec3&
	get_local_position() const
	{
		return m_pos;
	}

	const glm::vec3&
	get_local_rotation() const
	{
		return m_eular_rot;
	}

	const glm::mat4&
	get_model_matrix() const
	{
		return model_matrix;
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

	// update transform if it changed
	void
	update_self_and_child()
	{
		if (transform.is_dirty)
		{
			force_update_self_child();
		}

		for (auto&& child : children)
		{
			child->update_self_and_child();
		}
	}

	void
	force_update_self_child()
	{
		if (parent)
			transform.compute_model_matrix(parent->transform.get_model_matrix());
		else
			transform.compute_model_matrix();

		for (auto&& child : children)
		{
			child->force_update_self_child();
		}
	}
};