#include "../pch.h"
#include "time_module.h"

template<typename T>
constexpr auto NsToSeconds(T ns) { return ((ns) / 1000000000.0); }

SDL_AppResult TimeModule::Init()
{
	return SDL_APP_CONTINUE;
}

void TimeModule::Cleanup()
{
}

std::string TimeModule::GetName()
{
	return "TimeSystem";
}

bool TimeModule::Update()
{
	uint64_t ticksNs = SDL_GetTicksNS();
	time = NsToSeconds(ticksNs);
	if (lastTickTime == 0.0)
	{
		// Avoids a much larger initial deltaTime value
		lastTickTime = time;
	}
	deltaTime = time - lastTickTime;

	constexpr double_t TICK_COOLDOWN = 1.0 / 60.0;
	if (deltaTime < TICK_COOLDOWN)
	{
		return false;
	}

	lastTickTime = time;
	tickCount++;
	return true;
}

ENGINE_API uint64_t TimeModule::GetTickCount()
{
	return tickCount;
}

ENGINE_API double_t TimeModule::GetDeltaTime()
{
	return deltaTime;
}

ENGINE_API double_t TimeModule::GetTime()
{
	return time;
}
