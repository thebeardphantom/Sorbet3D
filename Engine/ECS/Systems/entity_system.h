#pragma once
#include <cmath>
#include <entt/entt.hpp>
#include "../../engine_api.h"

namespace sorbengine::ecs::systems
{
	class entity_system
	{
	public:
		ENGINE_API virtual ~entity_system() = default;

		struct tick_args
		{
			double_t delta_time;
			entt::registry& registry;
		};

		ENGINE_API virtual void init();
		ENGINE_API virtual void tick(tick_args& args);
		ENGINE_API virtual std::string get_name() = 0;
	};
}
