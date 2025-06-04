#include "engine.h"
#include "logging.h"

namespace sorbengine
{
	engine_instance& engine::get_instance()
	{
		static engine_instance instance;
		return instance;
	}

	SDL_AppResult engine::on_app_init(const SDL_LogPriority global_log_priority)
	{
		logging::init(global_log_priority);
		return get_instance().init();
	}

	SDL_AppResult engine::on_app_event(const SDL_Event& event)
	{
		return get_instance().receive_event(event);
	}

	SDL_AppResult engine::on_app_iterate()
	{
		return get_instance().iterate();
	}

	void engine::on_app_quit()
	{
		get_instance().cleanup_and_shutdown();
	}

	entt::dispatcher& engine::get_dispatcher()
	{
		return *get_instance().dispatcher_;
	}

	bool engine::get_is_quitting()
	{
		return get_instance().is_quitting_;
	}
}
