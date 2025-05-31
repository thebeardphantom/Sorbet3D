#include "pch.h"
#include "game_instance.h"
#include "spinnable.h"
#include "spin_system.h"
#include "../Engine/engine.h"
#include "../Engine/engine_instance.h"
#include "../Engine/fast_noise_lite.h"
#include "../Engine/ECS/Components/camera.h"
#include "../Engine/ECS/Components/mesh_renderer.h"
#include "../Engine/ECS/Components/transform.h"
#include "../Engine/Modules/asset_module.h"
#include "../Engine/Modules/ecs_module.h"
#include "../Engine/Objects/mesh_cpu.h"

namespace demo_game
{
	void game_instance::init()
	{
		SDL_Log("game_instance initializing.");
		quit_callback_id_ = sorbet::engine::get_quit_event().subscribe([this]
		{
			this->quit();
		});

		sorbet::modules::asset_module& asset_module = sorbet::engine::get_module<sorbet::modules::asset_module>();
		const std::shared_ptr<sorbet::objects::mesh_cpu> mesh_cpu = asset_module.load_model("Engine/Models/monkey.fbx");


		auto& ecs_module = sorbet::engine::get_module<sorbet::modules::ecs_module>();
		auto& registry = ecs_module.get_registry();
		const auto camera_entity = registry.create();
		registry.emplace_or_replace<sorbet::ecs::components::camera>(camera_entity);
		registry.emplace_or_replace<sorbet::ecs::components::transform>(camera_entity);
		ecs_module.create_system<ecs::systems::spin_system>();

		fast_noise_lite fnl(SDL_rand_bits());
		fnl.SetFrequency(5.41324f);
		for (size_t i = 0; i < 1; i++)
		{
			const auto entity = registry.create();
			registry.emplace_or_replace<ecs::components::spinnable>(entity);
			auto& [mesh] = registry.emplace_or_replace<sorbet::ecs::components::mesh_renderer>(entity);
			mesh = mesh_cpu;

			auto& [local_position, local_rotation] = registry.emplace_or_replace<
				sorbet::ecs::components::transform>(entity);

			auto x = fnl.GetNoise(static_cast<float>(i), 0.0f);
			auto y = fnl.GetNoise(static_cast<float>(i), 1.0f);
			auto z = fnl.GetNoise(static_cast<float>(i), 2.0f);
			//local_position = {x, y, z};
			local_position = {0, 0, -2.0};
		}
	}

	void game_instance::quit() const
	{
		SDL_Log("game_instance quitting.");
		sorbet::engine::get_quit_event().unsubscribe(quit_callback_id_);
	}
}
