#include "../../pch.h"
#include "camera_system.h"
#include "../../engine.h"
#include "../../smath.h"
#include "../../Modules/render_module.h"
#include "../Components/camera.h"
#include "../Components/transform.h"

namespace sorbengine::ecs::systems
{
	void camera_system::init() {}

	void camera_system::tick(tick_args& args)
	{
		auto& render_module = engine::get_module<modules::render_module>();

		//const auto camera_only_view = args.registry.view<components::camera>(entt::exclude<components::transform>);
		//for (const auto entity : camera_only_view)
		//{
		//	auto& camera = camera_only_view.get<components::camera>(entity);
		//	const auto rotation = camera.get_pitch_yaw_rotation();
		//	render_module.set_view(camera.get_view_matrix(glm::vec3(), rotation));
		//	const glm::mat4 projection = camera.get_perspective_matrix();
		//	render_module.set_projection(projection);
		//}
		const auto camera_tform_view = args.registry.view<components::camera, components::transform>();
		for (const auto entity : camera_tform_view)
		{
			auto [camera, transform] = camera_tform_view.get<components::camera, components::transform>(entity);
			transform.set_local_rotation(camera.get_pitch_yaw_rotation());
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
