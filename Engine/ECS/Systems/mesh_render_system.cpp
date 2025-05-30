#include "../../pch.h"
#include "mesh_render_system.h"
#include "../../engine.h"
#include "../../Modules/render_module.h"
#include "../Components/mesh_renderer.h"
#include "../Components/transform.h"

namespace sorbengine::ecs::systems
{
	void mesh_render_system::init() {}

	void mesh_render_system::tick(tick_args& args)
	{
		auto& module = engine::get_module<modules::render_module>();

		const auto no_transform_view = args.registry.view<components::mesh_renderer>(
			entt::exclude<components::transform>);
		constexpr auto identity = glm::mat4(1.0f);
		for (const auto entity : no_transform_view)
		{
			auto& [mesh] = no_transform_view.get<components::mesh_renderer>(entity);
			render_command cmd(mesh);
			cmd.model_matrix = identity;
			module.submit(cmd);
		}

		const auto renderer_and_transform_view = args.registry.view<components::mesh_renderer, components::transform>();
		for (const auto entity : renderer_and_transform_view)
		{
			auto [mesh_renderer, transform] = renderer_and_transform_view.get<
				components::mesh_renderer,
				components::transform>(entity);
			render_command cmd(mesh_renderer.mesh);
			cmd.model_matrix = transform.get_trs_matrix();
			module.submit(cmd);
		}
	}

	std::string mesh_render_system::get_name()
	{
		return "mesh_render_system";
	}
}
