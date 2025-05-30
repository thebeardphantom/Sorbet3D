#include "pch.h"
#include "engine.h"
#include "logging.h"

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
	return get_instance().process_event(event);
}

SDL_AppResult engine::on_app_iterate()
{
	return get_instance().iterate();
}

void engine::on_app_quit()
{
	get_instance().cleanup_and_shutdown();
}

event<>& engine::get_update_event()
{
	return get_instance().update_event_;
}

event<const SDL_Event&>& engine::get_sdl_event_event()
{
	return get_instance().sdl_event_event_;
}

event<>& engine::get_quit_event()
{
	return get_instance().quit_event_;
}

bool engine::get_is_quitting()
{
	return get_instance().is_quitting_;
}
