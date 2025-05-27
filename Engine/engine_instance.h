#pragma once
#include "engine_api.h"
#include "entity_system.h"
#include "event.h"
#include "Modules/engine_module.h"
#include <glad/glad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <format>
#include <Windows.h>

class EngineInstance
{
public:
	ENGINE_API static SDL_AppResult OnAppInit();

	ENGINE_API static SDL_AppResult OnAppEvent(const SDL_Event* event);

	ENGINE_API static SDL_AppResult OnAppIterate();

	ENGINE_API static void OnAppQuit();

	ENGINE_API static EngineInstance& GetInstance();

	ENGINE_API static bool IsShuttingDown();

	template <class T>
	T& GetEngineModule()
	{
		for (auto& system : modules)
		{
			if (auto* casted = dynamic_cast<T*>(system.get()))
			{
				return *casted;
			}
		}
		throw std::exception(std::format("Could not find system by type %s.", typeid(T).name()).c_str());
	}

	Event<double_t> TickEvent;

	Event<> QuitEvent;

private:
	static bool isQuitting;

	std::vector<std::unique_ptr<EngineModule>> modules;

	SDL_AppResult Init();

	SDL_AppResult ProcessEvent(const SDL_Event* event);

	SDL_AppResult Iterate();

	void Update();

	void Render();

	void Cleanup();
};

