#include "sorbengine/modules/engine_module.h"

namespace sorbengine::modules
{
	SDL_AppResult engine_module::init()
	{
		return SDL_APP_CONTINUE;
	}

	void engine_module::collaborate() {}

	void engine_module::cleanup() {}

	void engine_module::shutdown() {}

	engine_module::event_receive_result engine_module::receive_event(const SDL_Event& event)
	{
		return {.is_event_used = false, .app_result = SDL_APP_CONTINUE};
	}
}
