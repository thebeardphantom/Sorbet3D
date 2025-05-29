#include "../pch.h"
#include "game_layer_module.h"

namespace modules
{
	SDL_AppResult game_layer_module::init()
	{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Loading Game.dll.");
		game_so_ = SDL_LoadObject("Game.dll");
		if (game_so_ == nullptr)
		{
			const char* error = SDL_GetError();
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to load Game.dll: %s.", error);
			return SDL_APP_FAILURE;
		}

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Locating game_entry_point.");

		const auto game_entry_point = SDL_LoadFunction(game_so_, "game_entry_point");
		if (game_entry_point == nullptr)
		{
			const char* error = SDL_GetError();
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Cannot locate game_entry_point in Game.dll: %s.", error);
			return SDL_APP_FAILURE;
		}

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Invoking game_entry_point.");
		game_entry_point();
		return SDL_APP_CONTINUE;
	}

	void game_layer_module::cleanup() {}

	void game_layer_module::shutdown()
	{
		if (game_so_ != nullptr)
		{
			SDL_UnloadObject(game_so_);
			game_so_ = nullptr;
		}
	}

	std::string game_layer_module::get_name()
	{
		return "game_layer_module";
	}
}
