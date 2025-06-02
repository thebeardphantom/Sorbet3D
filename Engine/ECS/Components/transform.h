#pragma once
#include <glm/glm.hpp>
#include "../../smath.h"

namespace sorbengine::ecs::components
{
	struct transform
	{
		glm::vec3 local_position = {};

		[[nodiscard]] ENGINE_API glm::vec3 get_forward() const;
		[[nodiscard]] ENGINE_API glm::vec3 get_up() const;
		[[nodiscard]] ENGINE_API glm::vec3 get_right() const;
		[[nodiscard]] ENGINE_API glm::vec3 get_euler_angles() const;
		ENGINE_API glm::quat get_local_rotation() const;
		ENGINE_API void set_local_rotation(glm::quat local_rotation);
		ENGINE_API void set_local_euler_angles(glm::vec3 euler_angles);
		ENGINE_API glm::mat4 get_trs_matrix() const;

	private:
		glm::quat local_rotation_ = smath::identity_quaternion;
	};
}
