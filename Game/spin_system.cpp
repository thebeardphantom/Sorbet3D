#include "pch.h"
#include "spin_system.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include "spinnable.h"
#include "../Engine/ECS/Components/transform.h"

using namespace sorbengine::ecs::systems;
using namespace sorbengine::ecs::components;
using namespace glm;

namespace demo_game::ecs::systems
{
	void spin_system::init() {}

	void spin_system::tick(tick_args& args)
	{
		const auto view = args.registry.view<transform, components::spinnable>();

		const auto dt_float = static_cast<float>(args.delta_time);
		constexpr auto axis = vec3(0.0f, 1.0f, 0.0f);
		for (const auto& entity : view)
		{
			auto [transform, spinnable] = view.get<sorbengine::ecs::components::transform,
				components::spinnable>(entity);
			const auto spin = angleAxis(radians(spinnable.speed * dt_float), axis);
			transform.local_rotation *= spin;
		}
	}

	std::string spin_system::get_name()
	{
		return "spin_system";
	}
}
