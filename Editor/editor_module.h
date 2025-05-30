#pragma once
#include "../Engine/Modules/engine_module.h"

class editor_module final : public modules::engine_module
{
public:
	SDL_AppResult init() override;
	void collaborate() override;
	void cleanup() override;
	void shutdown() override;
	std::string get_name() override;
};
