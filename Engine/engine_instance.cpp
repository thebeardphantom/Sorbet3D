#include "pch.h"
#include "engine_instance.h"
#include <SDL3/SDL_filesystem.h>
#include "enum_strings.h"
#include "logging.h"
#include "Events/engine_events.h"
#include "Modules/asset_module.h"
#include "Modules/ecs_module.h"
#include "Modules/editor_layer_module.h"
#include "Modules/game_layer_module.h"
#include "Modules/input_module.h"
#include "Modules/render_module.h"
#include "Modules/time_module.h"

using namespace sorbengine::modules;
using namespace sorbengine::events;

namespace sorbengine
{
	SDL_AppResult engine_instance::init()
	{
		SDL_Log("== init ==");
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		dispatcher_ = std::make_unique<entt::dispatcher>();

		create_module<time_module>(false);
		create_module<ecs_module>(false);
		create_module<asset_module>(false);
		create_module<input_module>(false);
		create_module<render_module>(false);
		create_module<game_layer_module>(false);
		create_module<editor_layer_module>(false);

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

	SDL_AppResult engine_instance::receive_event(const SDL_Event& event)
	{
		SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "engine_instance::receive_event()");
		if (event.type == SDL_EVENT_QUIT)
		{
			// end the program, reporting success to the OS.
			SDL_Log("engine_instance quitting");
			is_quitting_ = true;
			return SDL_APP_SUCCESS;
		}

		dispatcher_->trigger<receive_sdlevent_event>(receive_sdlevent_event{event});
		return SDL_APP_CONTINUE;
	}

	SDL_AppResult engine_instance::iterate()
	{
		SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "engine_instance::iterate()");
		update();
		render();
		return SDL_APP_CONTINUE;
	}

	void engine_instance::update() const
	{
		SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "engine_instance updating.");

		dispatcher_->trigger<void_event>(void_events::update);
		dispatcher_->update();
	}

	void engine_instance::render()
	{
		SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "engine_instance rendering.");

		auto& render_module = get_module<modules::render_module>();
		render_module.render();
	}

	void engine_instance::cleanup_and_shutdown()
	{
		dispatcher_->trigger<void_event>(quitting);
		dispatcher_.reset();
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
