#pragma once
#include <SDL3/SDL_init.h>

class EngineModule
{
public:
	virtual SDL_AppResult Init() = 0;
	virtual void Cleanup() = 0;
	virtual std::string GetName() = 0;
};