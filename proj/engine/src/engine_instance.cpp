#include "engine_instance.h"
#include "enum_strings.h"
#include "Events/engine_events.h"
#include "Modules/asset_module.h"
#include "Modules/config_module.h"
#include "Modules/ecs_module.h"
#include "Modules/editor_layer_module.h"
#include "Modules/game_layer_module.h"
#include "Modules/input_module.h"
#include "Modules/render_module.h"
#include "Modules/time_module.h"
#include "Modules/serialization_module.h"

using namespace sorbengine::modules;
using namespace sorbengine::events;

namespace sorbengine
{
	SDL_AppResult engine_instance::init()
	{
		SDL_Log("== init ==");
		dispatcher_ = std::make_unique<entt::dispatcher>();

		SDL_AppResult result = SDL_APP_CONTINUE;

		// Init config module first and early to allow other modules to access config
		auto& config_module = create_module_internal<modules::config_module>(false);
		init_module(config_module, result);
		if (result != SDL_APP_CONTINUE)
		{
			return result;
		}

		create_module<ecs_module>(false);
		create_module<time_module>(false);
		create_module<asset_module>(false);
		create_module<render_module>(false);
		create_module<game_layer_module>(false);
		create_module<serialization_module>(false);
		create_module<input_module>(false);
		create_module<editor_layer_module>(false);

		bool check_for_inits = true;
		while (check_for_inits)
		{
			check_for_inits = false;
			for (auto& val : registered_modules_)
			{
				if (init_module(val, result))
				{
					if (result != SDL_APP_CONTINUE)
					{
						return result;
					}
					check_for_inits = true;
				}
			}
		}

		for (const auto& val : registered_modules_)
		{
			SDL_Log("== %s::collaborate ==", val.module->get_name().c_str());
			val.module->collaborate();
		}

		SDL_Log("Init complete with result: %s", enum_strings::to_string(result).c_str());
		return result;
	}

	bool engine_instance::init_module(registered_module& rm, SDL_AppResult& app_result)
	{
		if (rm.has_called_init)
		{
			return false;
		}

		rm.has_called_init = true;
		SDL_Log("== %s::init ==", rm.module->get_name().c_str());
		app_result = rm.module->init();
		return true;
	}

	SDL_AppResult engine_instance::receive_event(const SDL_Event& event)
	{
		for (const auto& registered_module : registered_modules_)
		{
			auto [is_event_used, app_result] = registered_module.module->receive_event(event);
			if (is_event_used)
			{
				return app_result;
			}
		}
		SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "engine_instance::receive_event()");
		if (event.type == SDL_EVENT_QUIT)
		{
			// end the program, reporting success to the OS.
			SDL_Log("engine_instance quitting");
			is_quitting_ = true;
			return SDL_APP_SUCCESS;
		}

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
		SDL_Log("Cleaning up engine modules.");
		for (auto it = registered_modules_.rbegin(); it != registered_modules_.rend(); ++it)
		{
			const auto& rm = *it;
			SDL_Log("== %s::Cleanup ==", rm.module->get_name().c_str());
			rm.module->cleanup();
		}

		SDL_Log("Shutting down engine modules.");
		for (int i = static_cast<int>(registered_modules_.size()) - 1; i >= 0; i--)
		{
			const auto& rm = registered_modules_[i];
			SDL_Log("== %s::Shutdown ==", rm.module->get_name().c_str());
			rm.module->shutdown();
			delete_module(rm);
		}
	}

	void engine_instance::delete_module(const registered_module& rm)
	{
		const auto index = type_index_to_index_[rm.type_index];
		type_index_to_index_.erase(rm.type_index);
		registered_modules_.erase(registered_modules_.begin() + index);
	}
}
