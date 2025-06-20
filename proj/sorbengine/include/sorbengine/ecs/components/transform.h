#pragma once
#include <glm/glm.hpp>
#include "cereal/cereal.hpp"
#include "sorbengine/engine_api.h"
#include "sorbengine/serializable.h"
#include "sorbengine/smath.h"

namespace sorbengine::ecs::components
{
	struct transform final : serializable
	{
		glm::vec3 local_position = {};

		[[nodiscard]] ENGINE_API glm::vec3 get_forward() const;
		[[nodiscard]] ENGINE_API glm::vec3 get_up() const;
		[[nodiscard]] ENGINE_API glm::vec3 get_right() const;
		[[nodiscard]] ENGINE_API glm::vec3 get_local_euler_angles() const;
		ENGINE_API glm::quat get_local_rotation() const;
		ENGINE_API void set_local_rotation(glm::quat local_rotation);
		ENGINE_API void set_local_euler_angles(glm::vec3 euler_angles);
		ENGINE_API glm::mat4 get_trs_matrix() const;

		template <class Archive>
		void serialize(Archive& ar);

	private:
		glm::quat local_rotation_ = smath::identity_quaternion;
	};

	template <class Archive>
	void transform::serialize(Archive& ar)
	{
		ar(CEREAL_NVP(local_position.x), CEREAL_NVP(local_position.y), CEREAL_NVP(local_position.z));
		auto local_eulers = get_local_euler_angles();
		ar(CEREAL_NVP(local_eulers.x), CEREAL_NVP(local_eulers.y), CEREAL_NVP(local_eulers.z));
	}
}
