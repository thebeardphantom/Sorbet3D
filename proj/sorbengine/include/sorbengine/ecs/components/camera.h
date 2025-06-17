#pragma once
#include <SDL3/SDL_stdinc.h>
#include "sorbengine/ecs/components/transform.h"

namespace sorbengine::ecs::components
{
	struct camera
	{
		float_t fov = 60.0f;
		float_t pitch = 0.0f;
		float_t yaw = 0.0f;
		Sint32 priority = 0;
		bool is_active = false;
		bool is_enabled = true;

		glm::mat4 get_view_matrix(const transform& tform) const;
		glm::mat4 get_perspective_matrix(float_t aspect) const;
		glm::quat get_pitch_yaw_rotation() const;
	};
}
