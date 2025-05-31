#include "../../pch.h"
#include "camera_system.h"

#include "../../engine.h"
#include "../../Modules/render_module.h"
#include "../../Modules/time_module.h"
#include "../Components/camera.h"
#include "../Components/transform.h"

namespace sorbengine::ecs::systems
{
	void camera_system::init() {}

	void camera_system::tick(tick_args& args)
	{
		auto& render_module = engine::get_module<modules::render_module>();
		constexpr auto view_default = glm::mat4(1.0f);
		render_module.set_view(view_default);

		const auto camera_only_view = args.registry.view<components::camera>(entt::exclude<components::transform>);
		for (const auto entity : camera_only_view)
		{
			auto& camera = camera_only_view.get<components::camera>(entity);
			const glm::mat4 projection = camera.get_perspective_matrix();
			render_module.set_projection(projection);
		}

		const auto camera_tform_view = args.registry.view<components::camera, components::transform>();
		for (const auto entity : camera_tform_view)
		{
			auto [camera, transform] = camera_tform_view.get<components::camera, components::transform>(entity);
			render_module.set_view(transform.get_trs_matrix());

			camera.fov = glm::mix(30.0f, 90.0f, (glm::sin(engine::get_module<modules::time_module>().get_time() * 2.0) + 1.0) / 2.0);
			const glm::mat4 projection = camera.get_perspective_matrix();
			render_module.set_projection(projection);
		}
	}

	std::string camera_system::get_name()
	{
		return "camera_system";
	}
}
