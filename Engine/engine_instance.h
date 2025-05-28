#pragma once
#include <format>
#include <SDL3/SDL_init.h>
#include "engine_api.h"
#include "entity_system.h"
#include "event.h"
#include "Modules/engine_module.h"

class engine_instance
{
public:
	// Public Static Methods
	ENGINE_API static SDL_AppResult on_app_init();
	ENGINE_API static SDL_AppResult on_app_event(const SDL_Event* event);
	ENGINE_API static SDL_AppResult on_app_iterate();
	ENGINE_API static void on_app_quit();
	ENGINE_API static engine_instance& get_instance();
	ENGINE_API static bool is_shutting_down();

	// Public Methods
	template <class T>
	T& get_engine_module()
	{
		for (auto& system : modules_)
		{
			if (auto* casted = dynamic_cast<T*>(system.get()))
			{
				return *casted;
			}
		}
		throw std::exception(std::format("Could not find system by type %s.", typeid(T).name()).c_str());
	}

	// Public Fields
	event<double_t> tick_event;
	event<> quit_event;

private:
	// Private Fields
	static bool is_quitting_;
	std::vector<std::unique_ptr<modules::engine_module>> modules_;

	// Private Methods
	SDL_AppResult init();
	SDL_AppResult process_event(const SDL_Event* event);
	SDL_AppResult iterate();
	void update();
	void render();
	void cleanup_and_shutdown() const;
};
