#pragma once
#include "engine_module.h"

class TimeModule : public EngineModule
{
public:
	SDL_AppResult Init() override;
	void Cleanup() override;
	std::string GetName() override;
	bool Update();
	ENGINE_API uint64_t GetTickCount();
	ENGINE_API double_t GetDeltaTime();
	ENGINE_API double_t GetTime();

private:
	uint64_t tickCount = 0;
	double_t lastTickTime = 0;
	double_t time = 0;
	double_t deltaTime = 0;
};

