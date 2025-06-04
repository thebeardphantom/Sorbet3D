#pragma once
#include <entt/entt.hpp>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include "engine_api.h"

#define SORBET_FLIP_Z

namespace sorbengine::smath
{
	ENGINE_API void apply_projection_matrix_scaling(glm::mat4& projection_matrix);

	constexpr static auto forward = glm::vec3(
		0.0,
		0.0,
#ifdef SORBET_FLIP_Z
		1.0
#else
		- 1.0
#endif
	);
	constexpr auto up = glm::vec3(0.0, 1.0, 0.0);
	constexpr auto right = glm::vec3(1.0, 0.0, 0.0);
	constexpr auto identity_matrix = glm::mat4(1.0f);
	constexpr auto identity_quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}
