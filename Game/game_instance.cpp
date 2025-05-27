#include "pch.h"
#include "../Engine/engine_instance.h"
#include "../Engine/Modules/ecs_module.h"
#include "demo_system.h"
#include "game_instance.h"
#include <string>

GameInstance& GameInstance::GetInstance()
{
	static GameInstance instance;
	return instance;
}

void GameInstance::Initialize()
{
	SDL_Log("GameInstance initializing.");
	auto& engine = EngineInstance::GetInstance();
	quitCallbackId = engine.QuitEvent.Subscribe([this]() { this->Quit(); });

	auto& ecsModule = engine.GetEngineModule<ECSModule>();
	ecsModule.CreateSystem<DemoSystem>();

	auto ent = ecsModule.GetRegistry().create();
}

void GameInstance::Quit() const
{
	SDL_Log("GameInstance quitting.");
	auto& engine = EngineInstance::GetInstance();
	engine.QuitEvent.Unsubscribe(quitCallbackId);
}