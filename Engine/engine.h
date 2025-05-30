#pragma once
#include "engine_api.h"
#include "engine_instance.h"

class engine
{
public:
	ENGINE_API static SDL_AppResult on_app_init();
	ENGINE_API static SDL_AppResult on_app_event(const SDL_Event& event);
	ENGINE_API static SDL_AppResult on_app_iterate();
	ENGINE_API static void on_app_quit();

	template <class T>
	static T& get_engine_module()
	{
		return get_instance().get_engine_module<T>();
	}

	ENGINE_API static event<> get_quit_event();
	ENGINE_API static bool get_is_quitting();

private:
	ENGINE_API static engine_instance& get_instance();
};
