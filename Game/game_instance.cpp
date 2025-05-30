#include "pch.h"
#include "game_instance.h"
#include "spin_system.h"
#include "../Engine/engine.h"
#include "../Engine/engine_instance.h"
#include "../Engine/ECS/Components/mesh_renderer.h"
#include "../Engine/ECS/Components/transform.h"
#include "../Engine/ECS/Systems/mesh_render_system.h"
#include "../Engine/Modules/asset_module.h"
#include "../Engine/Modules/ecs_module.h"
#include "../Engine/Objects/mesh_cpu.h"

game_instance& game_instance::get_instance()
{
	static game_instance instance;
	return instance;
}

void game_instance::initialize()
{
	SDL_Log("game_instance initializing.");
	quit_callback_id_ = engine::get_quit_event().subscribe([this]
	{
		this->quit();
	});

	modules::asset_module& asset_module = engine::get_engine_module<modules::asset_module>();
	std::shared_ptr<objects::mesh_cpu> mesh_cpu = asset_module.load_model("Engine/Models/monkey.fbx");


	auto& ecs_module = engine::get_engine_module<modules::ecs_module>();
	ecs_module.create_system<ecs::systems::mesh_render_system>();
	ecs_module.create_system<spin_system>();

	for (size_t i = 0; i < 1; i++)
	{
		auto& registry = ecs_module.get_registry();
		const auto entity = registry.create();
		auto& [mesh] = registry.emplace_or_replace<ecs::components::mesh_renderer>(entity);
		mesh = mesh_cpu;

		auto& [local_position, local_rotation] = registry.emplace_or_replace<ecs::components::transform>(entity);
		local_position = {SDL_randf() * 2.0f - 1.0f, SDL_randf() * 2.0f - 1.0f, SDL_randf() * 2.0f - 1.0f};
	}
}

void game_instance::quit() const
{
	SDL_Log("game_instance quitting.");
	engine::get_quit_event().unsubscribe(quit_callback_id_);
}
