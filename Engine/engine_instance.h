#pragma once
#include <format>
#include <SDL3/SDL_init.h>
#include "event.h"
#include "Modules/engine_module.h"

class engine_instance
{
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

	// Private Fields
	std::vector<std::unique_ptr<modules::engine_module>> modules_;

	// Private Methods
	SDL_AppResult init();
	SDL_AppResult process_event(const SDL_Event& event);
	SDL_AppResult iterate();
	void update();
	void render();
	void cleanup_and_shutdown() const;
	event<> quit_event_;
	bool is_quitting_ = false;

	friend class engine;
};
