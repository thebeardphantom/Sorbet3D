#include "../pch.h"
#include "engine_module.h"

namespace sorbengine::modules
{
	SDL_AppResult engine_module::init()
	{
		return SDL_APP_CONTINUE;
	}

	void engine_module::collaborate() {}

	void engine_module::cleanup() {}

	void engine_module::shutdown() {}

	int8_t engine_module::get_priority()
	{
		return 0;
	}

	engine_module::event_receive_result engine_module::receive_event(const SDL_Event& event)
	{
		return {.is_event_used = false, .app_result = SDL_APP_CONTINUE};
	}
}
