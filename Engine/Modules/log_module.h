#pragma once
#include "engine_module.h"
#include <string>

class LogModule : public EngineModule
{
public:
	SDL_AppResult Init() override;
	void Cleanup() override;
	std::string GetName() override;
};

