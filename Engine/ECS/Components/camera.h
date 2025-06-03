#pragma once
#include <glm/fwd.hpp>
#include "transform.h"

namespace sorbengine::ecs::components
{
	struct camera
	{
		float_t fov = 60.0f;
		float_t pitch = 0.0f;
		float_t yaw = 0.0f;
		uint8_t priority = 0;

		glm::mat4 get_view_matrix(const transform& tform) const;
		glm::mat4 get_perspective_matrix() const;
		glm::quat get_pitch_yaw_rotation() const;
	};
}
