#pragma once
#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace sorbengine::ecs::components
{
	struct transform
	{
		glm::vec3 local_position = {};
		glm::quat local_rotation = glm::quat(1.0, 0.0, 0.0, 0.0);

		[[nodiscard]] glm::vec3 get_forward() const;
		[[nodiscard]] glm::vec3 get_up() const;
		[[nodiscard]] glm::vec3 get_right() const;
		[[nodiscard]] glm::vec3 get_euler_angles() const;
		void set_euler_angles(glm::vec3 euler_angles);
		glm::mat4 get_trs_matrix() const;
	};
}
