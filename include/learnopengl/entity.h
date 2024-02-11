#pragma once
#include <glm/glm.hpp>

struct Transform
{
	// space information
	//  local space information
	glm::vec3 pos = {0.0f, 0.0f, 0.0f};
	glm::vec3 eular_rot = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale = {1.0f, 1.0, 1.0f};

	// Global space information concatenate in matrix
	glm::mat4 model_matrix = glm::mat4(1.0f);
};