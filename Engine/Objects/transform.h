#pragma once
#include <glm/ext/quaternion_float.hpp>

namespace objects
{
	class transform
	{
	public:
		glm::vec3 position = {};
		glm::quat rotation = {};

		glm::vec3 get_forward() const;
		glm::vec3 get_up() const;
		glm::vec3 get_right() const;
		glm::vec3 get_euler_angles() const;
		void set_euler_angles(glm::vec3 euler_angles);
	};
}
