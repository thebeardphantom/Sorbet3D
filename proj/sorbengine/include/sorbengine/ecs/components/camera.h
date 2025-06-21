#pragma once
#include <cereal/cereal.hpp>
#include <SDL3/SDL_stdinc.h>
#include "sorbengine/reflectable.h"
#include "sorbengine/ecs/components/transform.h"

namespace sorbengine::ecs
{
	struct camera final
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

		template <class Archive>
		void serialize(Archive& ar);
	};

	template <class Archive>
	void camera::serialize(Archive& ar)
	{
		ar(CEREAL_NVP(fov), CEREAL_NVP(pitch), CEREAL_NVP(yaw), CEREAL_NVP(priority), CEREAL_NVP(is_enabled));
	}
}
