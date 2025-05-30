#pragma once
#include "../../engine_api.h"

namespace ecs::systems
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

		ENGINE_API virtual void init() = 0;
		ENGINE_API virtual void tick(tick_args& args) = 0;
		ENGINE_API virtual std::string get_name() = 0;
	};
}
