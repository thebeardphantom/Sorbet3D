// ReSharper disable CppInconsistentNaming
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL_main.h>
#include "../Engine/engine.h"
#include "../Engine/enum_strings.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	const SDL_AppResult result = engine::on_app_init(SDL_LOG_PRIORITY_INFO);
	if (result != SDL_APP_CONTINUE)
	{
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"engine::on_app_init() returned %s",
			enum_strings::to_string(result).c_str());
	}
	return result;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	const SDL_AppResult result = engine::on_app_event(*event);
	if (result == SDL_APP_FAILURE)
	{
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"engine::on_app_event() returned %s",
			enum_strings::to_string(result).c_str());
	}
	return result;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	const SDL_AppResult result = engine::on_app_iterate();
	if (result != SDL_APP_CONTINUE)
	{
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"engine::on_app_iterate() returned %s",
			enum_strings::to_string(result).c_str());
	}
	return result;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	engine::on_app_quit();
}
