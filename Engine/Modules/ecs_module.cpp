#include "../pch.h"
#include "ecs_module.h"
#include "time_module.h"
#include "../engine_instance.h"

namespace modules
{
	SDL_AppResult ecs_module::init()
	{
		return SDL_APP_CONTINUE;
	}

	void ecs_module::cleanup()
	{
		entity_systems_.clear();
	}

	void ecs_module::shutdown() {}

	std::string ecs_module::get_name()
	{
		return "ecs_module";
	}

	void ecs_module::tick()
	{
		const auto& time_module = engine_instance::get_instance().get_engine_module<modules::time_module>();
		entity_system::tick_args tick_args =
		{
			.delta_time = time_module.get_delta_time(),
			.registry = registry_
		};
		for (const auto& system : entity_systems_)
		{
			system->tick(tick_args);
		}
	}

	ENGINE_API entt::registry& ecs_module::get_registry()
	{
		return registry_;
	}
}
