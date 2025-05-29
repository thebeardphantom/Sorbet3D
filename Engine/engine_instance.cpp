#include "pch.h"
#include "engine_instance.h"
#include <SDL3/SDL_filesystem.h>
#include "editor_layer_module.h"
#include "enum_strings.h"
#include "logging.h"
#include "Modules/asset_module.h"
#include "Modules/ecs_module.h"
#include "Modules/game_layer_module.h"
#include "Modules/render_module.h"
#include "Modules/time_module.h"

bool engine_instance::is_quitting_ = false;

SDL_AppResult engine_instance::on_app_init()
{
	logging::init();
	return get_instance().init();
}

SDL_AppResult engine_instance::on_app_event(const SDL_Event* event)
{
	return get_instance().process_event(event);
}

SDL_AppResult engine_instance::on_app_iterate()
{
	return get_instance().iterate();
}

void engine_instance::on_app_quit()
{
	get_instance().cleanup_and_shutdown();
}

ENGINE_API engine_instance& engine_instance::get_instance()
{
	static engine_instance instance;
	return instance;
}

ENGINE_API bool engine_instance::is_shutting_down()
{
	return is_quitting_;
}

SDL_AppResult engine_instance::init()
{
	SDL_Log("== init ==");

	is_quitting_ = false;

	modules_.push_back(std::make_unique<modules::time_module>());
	modules_.push_back(std::make_unique<modules::render_module>());
	modules_.push_back(std::make_unique<modules::ecs_module>());
	modules_.push_back(std::make_unique<modules::asset_module>());
	modules_.push_back(std::make_unique<modules::game_layer_module>());
	modules_.push_back(std::make_unique<modules::editor_layer_module>());

	SDL_AppResult result = SDL_APP_CONTINUE;
	for (const auto& module : modules_)
	{
		SDL_Log("== %s::init ==", module->get_name().c_str());
		result = module->init();
		if (result != SDL_APP_CONTINUE)
		{
			return result;
		}
	}

	SDL_Log("Init complete with result: %s", enum_strings::to_string(result).c_str());
	return result;
}

SDL_AppResult engine_instance::process_event(const SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT)
	{
		// end the program, reporting success to the OS.
		SDL_Log("engine_instance quitting");
		is_quitting_ = true;
		quit_event();
		return SDL_APP_SUCCESS;
	}
	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		auto& render_module = get_engine_module<modules::render_module>();
		if (event->key.key == SDLK_F1)
		{
			render_module.wireframe_mode = !render_module.wireframe_mode;
		}
		else if (event->key.key == SDLK_F2)
		{
			render_module.normals_mode = !render_module.normals_mode;
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult engine_instance::iterate()
{
	auto& time_module = get_engine_module<modules::time_module>();
	const bool should_tick = time_module.update();

	if (should_tick)
	{
		update();
		render();
	}
	return SDL_APP_CONTINUE;
}

void engine_instance::update()
{
	SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "engine_instance ticking.");

	auto& ecs_module = get_engine_module<modules::ecs_module>();
	ecs_module.tick();

	const auto& time_module = get_engine_module<modules::time_module>();
	tick_event(time_module.get_delta_time());
}

void engine_instance::render()
{
	auto& render_module = get_engine_module<modules::render_module>();
	render_module.render();
}

void engine_instance::cleanup_and_shutdown() const
{
	SDL_Log("Cleaning up engine instance.");
	for (const auto& module : modules_)
	{
		SDL_Log("== %s::Cleanup ==", module->get_name().c_str());
		module->cleanup();
	}

	SDL_Log("Shutting down engine instance.");
	for (const auto& module : modules_)
	{
		SDL_Log("== %s::Shutdown ==", module->get_name().c_str());
		module->shutdown();
	}
}
