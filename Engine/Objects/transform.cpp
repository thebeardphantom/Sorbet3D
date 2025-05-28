#include "../pch.h"
#include "transform.h"
#include <glm/gtc/quaternion.hpp>

namespace objects
{
	glm::vec3 transform::get_forward() const
	{
		auto fwd = glm::vec3(0, 0, 1);
		fwd = rotation * fwd;
		return fwd;
	}

	glm::vec3 transform::get_up() const
	{
		auto up = glm::vec3(0, 1, 0);
		up = rotation * up;
		return up;
	}

	glm::vec3 transform::get_right() const
	{
		auto right = glm::vec3(1, 0, 0);
		right = rotation * right;
		return right;
	}

	glm::vec3 transform::get_euler_angles() const
	{
		return glm::eulerAngles(rotation);
	}

	void transform::set_euler_angles(const glm::vec3 euler_angles)
	{
		rotation = glm::quat(euler_angles);
	}
}
