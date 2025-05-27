#include "pch.h"
#include "engine_instance.h"
#include "enum_strings.h"
#include "Modules/asset_module.h"
#include "Modules/ecs_module.h"
#include "Modules/game_layer_module.h"
#include "Modules/render_module.h"
#include "Modules/time_module.h"
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include "Modules/log_module.h"

bool EngineInstance::isQuitting = false;

SDL_AppResult EngineInstance::OnAppInit()
{
	return GetInstance().Init();
}

SDL_AppResult EngineInstance::OnAppEvent(const SDL_Event* event)
{
	return GetInstance().ProcessEvent(event);
}

SDL_AppResult EngineInstance::OnAppIterate()
{
	return GetInstance().Iterate();
}

void EngineInstance::OnAppQuit()
{
	GetInstance().Cleanup();
}

ENGINE_API EngineInstance& EngineInstance::GetInstance()
{
	static EngineInstance engineInstance;
	return engineInstance;
}

ENGINE_API bool EngineInstance::IsShuttingDown()
{
	return isQuitting;
}

SDL_AppResult EngineInstance::Init()
{
	SDL_Log("== Init ==");

	isQuitting = false;

	modules.push_back(std::make_unique<LogModule>());
	modules.push_back(std::make_unique<TimeModule>());
	modules.push_back(std::make_unique<RenderModule>());
	modules.push_back(std::make_unique<GameLayerModule>());
	modules.push_back(std::make_unique<ECSModule>());
	modules.push_back(std::make_unique<AssetModule>());

	SDL_AppResult result = SDL_APP_CONTINUE;
	for (auto& module : modules)
	{
		SDL_Log("== %s::Init ==", module->GetName().c_str());
		result = module->Init();
		if (result != SDL_APP_CONTINUE)
		{
			return result;
		}
	}

	SDL_Log("Init complete with result: %s", EnumStrings::ToString(result).c_str());
	return result;
}

SDL_AppResult EngineInstance::ProcessEvent(const SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT)
	{
		// end the program, reporting success to the OS.
		SDL_Log("EngineInstance quitting");
		isQuitting = true;
		QuitEvent();
		return SDL_APP_SUCCESS;
	}
	else if (event->type == SDL_EVENT_KEY_DOWN)
	{
		if (event->key.key == SDLK_F1)
		{
			auto& renderModule = GetEngineModule<RenderModule>();
			renderModule.WireframeMode = !renderModule.WireframeMode;
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult EngineInstance::Iterate()
{
	auto& timeModule = GetEngineModule<TimeModule>();
	bool shouldTick = timeModule.Update();

	if (shouldTick)
	{
		Update();
		Render();
	}
	return SDL_APP_CONTINUE;
}

void EngineInstance::Update()
{
	SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "EngineInstance ticking.");

	auto& ecsModule = GetEngineModule<ECSModule>();
	ecsModule.Tick();

	auto& timeModule = GetEngineModule<TimeModule>();
	TickEvent(timeModule.GetDeltaTime());
}

void EngineInstance::Render()
{
	auto& renderModule = GetEngineModule<RenderModule>();
	renderModule.Render();
}

void EngineInstance::Cleanup()
{
	SDL_Log("Cleaning engine instance.");
	for (auto& module : modules)
	{
		SDL_Log("== %s::Cleanup ==", module->GetName().c_str());
		module->Cleanup();
	}
}