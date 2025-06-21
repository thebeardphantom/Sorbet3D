#include "sorbengine/modules/ecs_module.h"
#include "cereal/archives/json.hpp"
#include "cereal/archives/xml.hpp"
#include "sorbengine/engine.h"
#include "sorbengine/ecs/components/camera.h"
#include "sorbengine/ecs/systems/camera_system.h"
#include "sorbengine/ecs/systems/mesh_render_system.h"
#include "sorbengine/events/engine_events.h"
#include "sorbengine/modules/time_module.h"

using namespace sorbengine::events;
using namespace sorbengine::ecs;

namespace sorbengine::modules
{
	SDL_AppResult ecs_module::init()
	{
		auto& dispatcher = engine::get_dispatcher();
		dispatcher.sink<void_event>(update).connect<&ecs_module::on_update>(this);
		create_system<mesh_render_system>();
		create_system<camera_system>();
		{
			transform tform;
			cereal::XMLOutputArchive xml(std::cout);
			cereal::JSONOutputArchive json(std::cout);
			auto nvp = utility::get_obj_nvp<transform>(tform);
			xml(nvp);
			json(nvp);
		}
		std::cout.flush();
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
			if (!system->enabled)
			{
				continue;
			}

			system->tick(tick_args);
		}
	}

	entt::registry& ecs_module::get_registry() const
	{
		return *registry_;
	}
}
