#pragma once
#include <glm/matrix.hpp>

namespace sorbengine::ecs::components
{
	struct camera
	{
		float_t fov = 60.0f;

		glm::mat4 get_perspective_matrix() const;
	};
}
