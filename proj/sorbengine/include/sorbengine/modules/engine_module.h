#pragma once
#include <string>
#include <SDL3/SDL_init.h>
#include "sorbengine/engine_api.h"

namespace sorbengine::modules
{
	class engine_module
	{
	public:
		struct event_receive_result
		{
			bool is_event_used = false;
			SDL_AppResult app_result = SDL_APP_CONTINUE;
		};

		ENGINE_API virtual ~engine_module() = default;
		ENGINE_API virtual SDL_AppResult init();
		ENGINE_API virtual void collaborate();
		ENGINE_API virtual void cleanup();
		ENGINE_API virtual void shutdown();
		ENGINE_API virtual event_receive_result receive_event(const SDL_Event& event);
		ENGINE_API virtual std::string get_name() = 0;
	};
}
