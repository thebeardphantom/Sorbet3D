#include "../pch.h"
#include "ecs_module.h"
#include "time_module.h"
#include "../engine.h"
#include "../ECS/Systems/camera_system.h"
#include "../ECS/Systems/mesh_render_system.h"

namespace sorbengine::modules
{
	SDL_AppResult ecs_module::init()
	{
		create_system<ecs::systems::mesh_render_system>();
		create_system<ecs::systems::camera_system>();
		return SDL_APP_CONTINUE;
	}

	void ecs_module::collaborate() {}

	void ecs_module::cleanup()
	{
		registry_.reset();
		entity_systems_.clear();
	}

	void ecs_module::shutdown() {}

	std::string ecs_module::get_name()
	{
		return "ecs_module";
	}

	void ecs_module::tick() const
	{
		const auto& time_module = engine::get_module<modules::time_module>();
		ecs::systems::entity_system::tick_args tick_args =
		{
			.delta_time = time_module.get_delta_time(),
			.registry = *registry_
		};
		for (const auto& system : entity_systems_)
		{
			system->tick(tick_args);
		}
	}

	ENGINE_API entt::registry& ecs_module::get_registry() const
	{
		return *registry_;
	}
}
