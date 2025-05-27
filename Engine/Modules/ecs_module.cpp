#include "../pch.h"
#include "../engine_instance.h"
#include "ecs_module.h"
#include "time_module.h"

SDL_AppResult ECSModule::Init()
{
	return SDL_APP_CONTINUE;
}

void ECSModule::Cleanup()
{
}

std::string ECSModule::GetName()
{
	return "ECSSystem";
}

void ECSModule::Tick()
{
	auto& timeModule = EngineInstance::GetInstance().GetEngineModule<TimeModule>();
	EntitySystem::TickArgs tickArgs = { timeModule.GetDeltaTime(), registry };
	for (auto& system : entitySystems)
	{
		system->Tick(tickArgs);
	}
}

ENGINE_API entt::registry& ECSModule::GetRegistry()
{
	return registry;
}
