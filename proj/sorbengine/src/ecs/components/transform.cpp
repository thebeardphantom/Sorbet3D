#include <cereal/access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sorbengine/smath.h>
#include <sorbengine/ecs/components/transform.h>

namespace sorbengine::ecs
{
	glm::vec3 transform::get_forward() const
	{
		return local_rotation_ * utility::forward;
	}

	glm::vec3 transform::get_up() const
	{
		return local_rotation_ * utility::up;
	}

	glm::vec3 transform::get_right() const
	{
		return local_rotation_ * utility::right;
	}

	glm::vec3 transform::get_local_euler_angles() const
	{
		const glm::vec3 eulers = glm::eulerAngles(local_rotation_);
		return glm::degrees(eulers);
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
		const glm::mat4 translation_matrix = glm::translate(utility::identity_matrix, local_position);
		const glm::mat<4, 4, float> rotation_matrix = mat4_cast(local_rotation_);
		return translation_matrix * rotation_matrix;
	}
}
