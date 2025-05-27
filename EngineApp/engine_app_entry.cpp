#define SDL_MAIN_USE_CALLBACKS 1

#include "../Engine/engine_instance.h"
#include "../Engine/enum_strings.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_AppResult result = EngineInstance::OnAppInit();
    if (result != SDL_AppResult::SDL_APP_CONTINUE)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "EngineInstance::OnAppInit() returned %s",
            EnumStrings::ToString(result).c_str());
    }
    return result;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    SDL_AppResult result = EngineInstance::OnAppEvent(event);
    if (result == SDL_AppResult::SDL_APP_FAILURE)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "EngineInstance::OnAppEvent() returned %s",
            EnumStrings::ToString(result).c_str());
    }
    return result;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_AppResult result = EngineInstance::OnAppIterate();
    if (result != SDL_AppResult::SDL_APP_CONTINUE)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION, 
            "EngineInstance::OnAppIterate() returned %s", 
            EnumStrings::ToString(result).c_str());
    }
    return result;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    EngineInstance::OnAppQuit();
}