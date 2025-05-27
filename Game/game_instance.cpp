#include "pch.h"
#include "game_instance.h"
#include "demo_system.h"
#include "../Engine/engine_instance.h"
#include "../Engine/Modules/ecs_module.h"

game_instance& game_instance::get_instance()
{
	static game_instance instance;
	return instance;
}

void game_instance::initialize()
{
	SDL_Log("game_instance initializing.");
	auto& engine = engine_instance::get_instance();
	quit_callback_id_ = engine.quit_event.subscribe([this]
	{
		this->quit();
	});

	auto& ecs_module = engine.get_engine_module<modules::ecs_module>();
	ecs_module.create_system<demo_system>();

	auto ent = ecs_module.get_registry().create();
}

void game_instance::quit() const
{
	SDL_Log("game_instance quitting.");
	auto& engine = engine_instance::get_instance();
	engine.quit_event.unsubscribe(quit_callback_id_);
}
