#include "game_instance.h"
#include <sorbengine/engine.h>
#include <sorbengine/engine_instance.h>
#include <sorbengine/fast_noise_lite.h>
#include <sorbengine/ecs/components/camera.h>
#include <sorbengine/ecs/components/mesh_renderer.h>
#include <sorbengine/ecs/components/transform.h>
#include <sorbengine/events/engine_events.h>
#include <sorbengine/modules/asset_module.h>
#include <sorbengine/modules/ecs_module.h>
#include "spinnable.h"
#include "spin_system.h"

using namespace sorbengine;
using namespace sorbengine::ecs;
using namespace sorbengine::ecs::components;
using namespace demo_game::ecs::components;
using namespace demo_game::ecs::systems;

namespace demo_game
{
	void game_instance::init()
	{
		SDL_Log("game_instance initializing.");
		auto& dispatcher = engine::get_dispatcher();
		dispatcher.sink<events::void_event>(quitting).connect<&game_instance::on_quitting>(this);

		auto& asset_module = engine::get_module<modules::asset_module>();


		// Create ecs systems
		auto& ecs_module = engine::get_module<modules::ecs_module>();
		ecs_module.create_system<spin_system>();

		// Create camera entity
		auto& registry = ecs_module.get_registry();
		const auto camera_entity = registry.create();
		registry.emplace_or_replace<camera>(camera_entity);
		registry.emplace_or_replace<transform>(camera_entity);

		// Create monkey entities
		const auto monkey_mesh_asset = asset_module.load_model("Engine/Models/monkey.fbx");
		fast_noise_lite fnl(SDL_rand_bits());
		fnl.SetFrequency(5.41324f);
		for (size_t i = 0; i < 1; i++)
		{
			const auto monkey_entity = registry.create();
			registry.emplace_or_replace<spinnable>(monkey_entity);
			auto& [monkey_mesh] = registry.get_or_emplace<mesh_renderer>(monkey_entity);
			monkey_mesh = monkey_mesh_asset;

			auto& tform = registry.emplace_or_replace<transform>(monkey_entity);

			auto x = fnl.GetNoise(static_cast<float>(i), 0.0f);
			auto y = fnl.GetNoise(static_cast<float>(i), 1.0f);
			auto z = fnl.GetNoise(static_cast<float>(i), 2.0f);
			//local_position = {x, y, z};
			tform.local_position = {0, 0, 0.0};
		}

		// Create room entity
		const auto room_entity = registry.create();
		auto& [room_mesh] = registry.get_or_emplace<mesh_renderer>(room_entity);
		room_mesh = asset_module.load_model("Engine/Models/debug_room.fbx");
	}

	void game_instance::on_quitting()
	{
		SDL_Log("game_instance on_quitting.");
	}
}
