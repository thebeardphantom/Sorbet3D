#include "../../pch.h"
#include "transform.h"
#include <glm/gtc/quaternion.hpp>

namespace sorbengine::ecs::components
{
	glm::vec3 transform::get_forward() const
	{
		auto fwd = glm::vec3(0, 0, 1);
		fwd = local_rotation * fwd;
		return fwd;
	}

	glm::vec3 transform::get_up() const
	{
		auto up = glm::vec3(0, 1, 0);
		up = local_rotation * up;
		return up;
	}

	glm::vec3 transform::get_right() const
	{
		auto right = glm::vec3(1, 0, 0);
		right = local_rotation * right;
		return right;
	}

	glm::vec3 transform::get_euler_angles() const
	{
		return glm::eulerAngles(local_rotation);
	}

	void transform::set_euler_angles(const glm::vec3 euler_angles)
	{
		local_rotation = glm::quat(euler_angles);
	}

	glm::mat4 transform::get_trs_matrix() const
	{
		const glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), local_position);
		const glm::mat<4, 4, float> rotation_matrix = mat4_cast(local_rotation);
		return translation_matrix * rotation_matrix;
	}
}
