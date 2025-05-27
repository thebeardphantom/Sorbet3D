#pragma once
#include "engine_module.h"
class GameLayerModule : public EngineModule
{
public:
	// Methods
	SDL_AppResult Init() override;
	void Cleanup() override;
	std::string GetName() override;

private:
	// Fields
	typedef void (*GameEntryPoint)();
	HMODULE gameModule = nullptr;
};

