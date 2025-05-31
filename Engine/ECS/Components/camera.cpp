#include "../../pch.h"
#include "camera.h"

namespace sorbengine::ecs::components
{
	glm::mat4 camera::get_perspective_matrix() const
	{
		return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
	}
}
