#include "sorbengine/ecs/systems/camera_system.h"
#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include "sorbengine/engine.h"
#include "sorbengine/smath.h"
#include "sorbengine/ecs/components/camera.h"
#include "sorbengine/ecs/components/transform.h"
#include "sorbengine/modules/render_module.h"

namespace sorbengine::ecs::systems
{
	void camera_system::tick(tick_args& args)
	{
		auto& render_module = engine::get_module<modules::render_module>();

		const auto camera_tform_view = args.registry.view<components::camera, components::transform>();
		entt::entity highest_priority_camera_entity = entt::null;
		Sint32 highest_priority_camera_priority = 0xFF;
		for (const auto entity : camera_tform_view)
		{
			auto [camera, transform] = camera_tform_view.get<components::camera, components::transform>(entity);
			camera.is_active = false;
			if (camera.is_enabled && camera.priority < highest_priority_camera_priority)
			{
				highest_priority_camera_entity = entity;
				highest_priority_camera_priority = camera.priority;
			}

			transform.set_local_rotation(camera.get_pitch_yaw_rotation());
		}

		if (highest_priority_camera_entity == entt::null)
		{
			render_module.set_view(smath::identity_matrix);
			render_module.set_projection(smath::identity_matrix);
		}
		else
		{
			auto [camera, transform] = camera_tform_view.get<components::camera, components::transform>(
				highest_priority_camera_entity);
			camera.is_active = true;
			render_module.set_view(camera.get_view_matrix(transform));


			float_t aspect = render_module.get_window_aspect();
			const glm::mat4 projection = camera.get_perspective_matrix(aspect);
			render_module.set_projection(projection);
		}
	}

	std::string camera_system::get_name()
	{
		return "camera_system";
	}
}
