#include "../pch.h"
#include "game_layer_module.h"

SDL_AppResult GameLayerModule::Init()
{
	SDL_Log("== InitGameLayer ==");

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Loading Game.dll.");
	gameModule = SDL_LoadObject("Game.dll");
	if (gameModule == nullptr)
	{
		const char* error = SDL_GetError();
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to load Game.dll: %s.", error);
		return SDL_APP_FAILURE;
	}

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Locating GameEntryPoint.");

	GameEntryPoint gameEntryPoint = (GameEntryPoint)SDL_LoadFunction(gameModule, "GameEntryPoint");
	if (gameEntryPoint == nullptr)
	{
		const char* error = SDL_GetError();
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Cannot locate GameEntryPoint in Game.dll: %s.", error);
		return SDL_APP_FAILURE;
	}

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Invoking GameEntryPoint.");
	gameEntryPoint();
	return SDL_APP_CONTINUE;
}

void GameLayerModule::Cleanup()
{
	if (gameModule != nullptr)
	{
		SDL_UnloadObject(gameModule);
		gameModule = nullptr;
	}
}

std::string GameLayerModule::GetName()
{
	return "GameLayerModule";
}
