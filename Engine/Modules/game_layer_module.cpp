#include "../pch.h"
#include "game_layer_module.h"

SDL_AppResult GameLayerModule::Init()
{
	SDL_Log("== InitGameLayer ==");

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Loading Game.dll.");
	gameModule = LoadLibrary(L"Game.dll");
	if (gameModule == nullptr)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to load Game.dll.");
		return SDL_APP_FAILURE;
	}

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Locating GameEntryPoint.");
	GameEntryPoint gameEntryPoint = (GameEntryPoint)GetProcAddress(gameModule, "GameEntryPoint");

	if (gameEntryPoint == nullptr)
	{
		FreeLibrary(gameModule);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Cannot locate GameEntryPoint in Game.dll.");
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
		FreeLibrary(gameModule);
	}
}

std::string GameLayerModule::GetName()
{
	return "GameLayerSystem";
}
