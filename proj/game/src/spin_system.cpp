#include "spin_system.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <sorbengine/ecs/components/transform.h>
#include "spinnable.h"

using namespace sorbengine::ecs;
using namespace sorbengine::ecs;
using namespace glm;

namespace demo_game::ecs
{
	void spin_system::tick(tick_args& args)
	{
		const auto view = args.registry.view<transform, spinnable>();

		const auto dt_float = static_cast<float>(args.delta_time);
		constexpr auto axis = vec3(0.0f, 1.0f, 0.0f);
		for (const auto& entity : view)
		{
			auto [transform, spinnable] = view.get<sorbengine::ecs::transform, ecs::spinnable>(entity);
			const auto spin = angleAxis(radians(spinnable.speed * dt_float), axis);
			transform.set_local_rotation(transform.get_local_rotation() * spin);
		}
	}

	std::string spin_system::get_name()
	{
		return "spin_system";
	}
}
