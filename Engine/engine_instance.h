#pragma once
#include <format>
#include <memory>
#include <vector>
#include <entt/entt.hpp>
#include <SDL3/SDL_init.h>
#include "Modules/engine_module.h"

namespace sorbengine
{
	class engine_instance
	{
		struct registered_module
		{
			bool is_external = false;
			bool has_called_init = false;
			std::unique_ptr<modules::engine_module> module;
		};

		template <class T>
		T& create_module(const bool is_external)
		{
			T* new_system_ptr = new T();
			registered_modules_.push_back({is_external, false, std::unique_ptr<T>(new_system_ptr)});
			return *new_system_ptr;
		}

		template <class T>
		T& get_module()
		{
			for (auto& [is_external, has_called_init, module] : registered_modules_)
			{
				if (auto* casted = dynamic_cast<T*>(module.get()))
				{
					return *casted;
				}
			}
			throw std::exception(std::format("Could not find system by type %s.", typeid(T).name()).c_str());
		}

		// Private Fields
		std::vector<registered_module> registered_modules_;

		// Private Methods
		SDL_AppResult init();
		SDL_AppResult receive_event(const SDL_Event& event);
		SDL_AppResult iterate();
		void update() const;
		void render();
		void cleanup_and_shutdown();
		void cleanup_and_shutdown_modules(bool external_modules);
		std::unique_ptr<entt::dispatcher> dispatcher_ = nullptr;
		bool is_quitting_ = false;

		friend class engine;
	};
}
