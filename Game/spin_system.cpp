#include "pch.h"
#include "spin_system.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include "../Engine/ECS/Components/transform.h"

void spin_system::init() {}

void spin_system::tick(tick_args& args)
{
	const auto transform_view = args.registry.view<ecs::components::transform>();
	const auto spin = glm::angleAxis(
		glm::radians(90.0f * static_cast<float>(args.delta_time)),
		glm::vec3(0.0f, 1.0f, 0.0f));
	for (const auto& entity : transform_view)
	{
		auto& [local_position, local_rotation] = transform_view.get<ecs::components::transform>(entity);
		local_rotation = local_rotation * spin;
	}
}

std::string spin_system::get_name()
{
	return "spin_system";
}
