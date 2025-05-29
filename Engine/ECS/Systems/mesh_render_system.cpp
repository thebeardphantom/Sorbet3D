#include "../../pch.h"
#include "mesh_render_system.h"

#include "../../engine_instance.h"
#include "../../Modules/render_module.h"
#include "../Components/mesh_renderer.h"
#include "../Components/transform.h"

void ecs::systems::mesh_render_system::init() {}

void ecs::systems::mesh_render_system::tick(tick_args& args)
{
	auto& module = engine_instance::get_instance().get_engine_module<modules::render_module>();

	const auto no_transform_view = args.registry.view<components::mesh_renderer>(entt::exclude<components::transform>);
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
		auto& [mesh] = renderer_and_transform_view.get<components::mesh_renderer>(entity);
		auto& transform = renderer_and_transform_view.get<components::transform>(entity);
		render_command cmd(mesh);
		cmd.model_matrix = transform.get_trs_matrix();
		module.submit(cmd);
	}
}

std::string ecs::systems::mesh_render_system::get_name()
{
	return "mesh_render_system";
}
