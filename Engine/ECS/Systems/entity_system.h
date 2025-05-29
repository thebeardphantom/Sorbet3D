#pragma once
#include <SDL3/SDL_log.h>

namespace ecs::systems
{
	class entity_system
	{
	public:
		virtual ~entity_system()
		{
			// Optional: Add a log or breakpoint here
			SDL_Log("entity_system dtor called");
		}

		struct tick_args
		{
			double_t delta_time;
			entt::registry& registry;
		};

		virtual void init() = 0;
		virtual void tick(tick_args& args) = 0;
		virtual std::string get_name() = 0;
	};
}
