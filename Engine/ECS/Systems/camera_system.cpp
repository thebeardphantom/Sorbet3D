#include "../../pch.h"
#include "camera_system.h"
#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include "../../engine.h"
#include "../../smath.h"
#include "../../Modules/render_module.h"
#include "../Components/camera.h"
#include "../Components/transform.h"

namespace sorbengine::ecs::systems
{
	void camera_system::tick(tick_args& args)
	{
		auto& render_module = engine::get_module<modules::render_module>();

		const auto camera_tform_view = args.registry.view<components::camera, components::transform>();
		entt::entity highest_priority_camera_entity = entt::null;
		uint8_t highest_priority_camera_priority = 0xFF;
		for (const auto entity : camera_tform_view)
		{
			auto [camera, transform] = camera_tform_view.get<components::camera, components::transform>(entity);
			if (camera.priority < highest_priority_camera_priority)
			{
				highest_priority_camera_entity = entity;
				highest_priority_camera_priority = camera.priority;
			}

			transform.set_local_rotation(camera.get_pitch_yaw_rotation());
		}

		if (highest_priority_camera_entity != entt::null)
		{
			auto [camera, transform] = camera_tform_view.get<components::camera, components::transform>(
				highest_priority_camera_entity);
			render_module.set_view(camera.get_view_matrix(transform));

			const glm::mat4 projection = camera.get_perspective_matrix();
			render_module.set_projection(projection);
		}
	}

	std::string camera_system::get_name()
	{
		return "camera_system";
	}
}
