#include "mesh_render_system.h"
#include "sorbengine/engine.h"
#include "sorbengine/smath.h"
#include "sorbengine/ecs/components/mesh_renderer.h"
#include "sorbengine/ecs/components/transform.h"
#include "sorbengine/modules/render_module.h"

namespace sorbengine::ecs::systems
{
	void mesh_render_system::tick(tick_args& args)
	{
		auto& module = engine::get_module<modules::render_module>();

		const auto no_transform_view = args.registry.view<components::mesh_renderer>(
			entt::exclude<components::transform>);
		for (const auto entity : no_transform_view)
		{
			auto& [mesh] = no_transform_view.get<components::mesh_renderer>(entity);
			render_command cmd(mesh);
			cmd.model_matrix = smath::identity_matrix;
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
