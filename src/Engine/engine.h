#pragma once
#include <SDL3/SDL_log.h>
#include "engine_api.h"
#include "engine_instance.h"

namespace sorbengine
{
	class engine
	{
	public:
		ENGINE_API static SDL_AppResult on_app_init(SDL_LogPriority global_log_priority);
		ENGINE_API static SDL_AppResult on_app_event(const SDL_Event& event);
		ENGINE_API static SDL_AppResult on_app_iterate();
		ENGINE_API static void on_app_quit();
		template <class T>
		static T& create_module();
		template <class T>
		static T& get_module();
		ENGINE_API static entt::dispatcher& get_dispatcher();
		ENGINE_API static bool get_is_quitting();

	private:
		ENGINE_API static engine_instance& get_instance();
	};

	template <class T>
	T& engine::create_module()
	{
		return get_instance().create_module<T>(true);
	}

	template <class T>
	T& engine::get_module()
	{
		return get_instance().get_module<T>();
	}
}
