#include "../pch.h"
#include "ecs_module.h"
#include "time_module.h"
#include "../engine.h"
#include "../ECS/Systems/camera_system.h"
#include "../ECS/Systems/mesh_render_system.h"
#include "../Events/engine_events.h"

using namespace sorbengine::events;
using namespace sorbengine::ecs::systems;

namespace sorbengine::modules
{
	SDL_AppResult ecs_module::init()
	{
		auto& dispatcher = engine::get_dispatcher();
		dispatcher.sink<void_event>(update).connect<&ecs_module::on_update>(this);
		create_system<mesh_render_system>();
		create_system<camera_system>();
		return SDL_APP_CONTINUE;
	}

	void ecs_module::cleanup()
	{
		registry_.reset();
		entity_systems_.clear();
	}

	std::string ecs_module::get_name()
	{
		return "ecs_module";
	}

	void ecs_module::on_update() const
	{
		const auto& time_module = engine::get_module<modules::time_module>();
		entity_system::tick_args tick_args =
		{
			.delta_time = time_module.get_delta_time(),
			.registry = *registry_
		};
		for (const auto& system : entity_systems_)
		{
			system->tick(tick_args);
		}
	}

	entt::registry& ecs_module::get_registry() const
	{
		return *registry_;
	}
}
