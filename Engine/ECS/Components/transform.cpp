#include "../../pch.h"
#include "transform.h"
#include <glm/gtc/quaternion.hpp>
#include "../../smath.h"

namespace sorbengine::ecs::components
{
	glm::vec3 transform::get_forward() const
	{
		return local_rotation_ * smath::forward;
	}

	glm::vec3 transform::get_up() const
	{
		return local_rotation_ * smath::up;
	}

	glm::vec3 transform::get_right() const
	{
		return local_rotation_ * smath::right;
	}

	glm::vec3 transform::get_euler_angles() const
	{
		return glm::eulerAngles(local_rotation_);
	}

	glm::quat transform::get_local_rotation() const
	{
		return local_rotation_;
	}

	void transform::set_local_rotation(const glm::quat local_rotation)
	{
		local_rotation_ = normalize(local_rotation);
	}

	void transform::set_local_euler_angles(const glm::vec3 euler_angles)
	{
		local_rotation_ = glm::quat(euler_angles);
	}

	glm::mat4 transform::get_trs_matrix() const
	{
		const glm::mat4 translation_matrix = glm::translate(smath::identity_matrix, local_position);
		const glm::mat<4, 4, float> rotation_matrix = mat4_cast(local_rotation_);
		return translation_matrix * rotation_matrix;
	}
}
