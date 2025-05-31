#include "pch.h"
#include "engine_instance.h"
#include <SDL3/SDL_filesystem.h>
#include "enum_strings.h"
#include "logging.h"
#include "Modules/asset_module.h"
#include "Modules/ecs_module.h"
#include "Modules/editor_layer_module.h"
#include "Modules/game_layer_module.h"
#include "Modules/render_module.h"
#include "Modules/time_module.h"

namespace sorbet
{
	SDL_AppResult engine_instance::init()
	{
		SDL_Log("== init ==");
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		create_module<modules::time_module>(false);
		create_module<modules::render_module>(false);
		create_module<modules::ecs_module>(false);
		create_module<modules::asset_module>(false);
		create_module<modules::game_layer_module>(false);
		create_module<modules::editor_layer_module>(false);

		SDL_AppResult result = SDL_APP_CONTINUE;
		bool check_for_inits = true;
		while (check_for_inits)
		{
			check_for_inits = false;
			for (auto& [is_external, has_called_init, module] : registered_modules_)
			{
				if (has_called_init)
				{
					continue;
				}

				check_for_inits = true;
				has_called_init = true;
				SDL_Log("== %s::init ==", module->get_name().c_str());
				result = module->init();
				if (result != SDL_APP_CONTINUE)
				{
					return result;
				}
			}
		}

		for (const auto& [is_external, has_called_init, module] : registered_modules_)
		{
			SDL_Log("== %s::collaborate ==", module->get_name().c_str());
			module->collaborate();
		}

		SDL_Log("Init complete with result: %s", enum_strings::to_string(result).c_str());
		return result;
	}

	SDL_AppResult engine_instance::process_event(const SDL_Event& event)
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			// end the program, reporting success to the OS.
			SDL_Log("engine_instance quitting");
			is_quitting_ = true;
			quit_event_();
			return SDL_APP_SUCCESS;
		}
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			auto& render_module = get_module<modules::render_module>();
			if (event.key.key == SDLK_F1)
			{
				render_module.wireframe_mode = !render_module.wireframe_mode;
			}
			else if (event.key.key == SDLK_F2)
			{
				render_module.normals_mode = !render_module.normals_mode;
			}
		}

		sdl_event_event_(event);
		return SDL_APP_CONTINUE;
	}

	SDL_AppResult engine_instance::iterate()
	{
		auto& time_module = get_module<modules::time_module>();
		time_module.update();
		update();
		render();
		return SDL_APP_CONTINUE;
	}

	void engine_instance::update()
	{
		SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "engine_instance ticking.");

		const auto& ecs_module = get_module<modules::ecs_module>();
		ecs_module.tick();
		update_event_();
	}

	void engine_instance::render()
	{
		auto& render_module = get_module<modules::render_module>();
		render_module.render();
	}

	void engine_instance::cleanup_and_shutdown()
	{
		update_event_.clear();
		sdl_event_event_.clear();
		quit_event_.clear();
		cleanup_and_shutdown_modules(true);
		cleanup_and_shutdown_modules(false);
	}

	void engine_instance::cleanup_and_shutdown_modules(const bool external_modules)
	{
		const std::string src_str = external_modules ? "external" : "internal";
		SDL_Log("Cleaning up %s engine modules.", src_str.c_str());
		for (const auto& [is_external, has_called_init, module] : registered_modules_)
		{
			if (is_external == external_modules)
			{
				SDL_Log("== %s::Cleanup ==", module->get_name().c_str());
				module->cleanup();
			}
		}

		SDL_Log("Shutting down %s engine modules.", src_str.c_str());
		for (auto it = registered_modules_.begin(); it != registered_modules_.end();)
		{
			if (it->is_external == external_modules)
			{
				SDL_Log("== %s::Shutdown ==", it->module->get_name().c_str());
				it->module->shutdown();
				it = registered_modules_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

}