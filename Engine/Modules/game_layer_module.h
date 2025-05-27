#pragma once
#include <SDL3/SDL_loadso.h>
#include "engine_module.h"

namespace modules
{
	class game_layer_module : public engine_module
	{
	public:
		// Methods  
		SDL_AppResult init() override;
		void cleanup() override;
		std::string get_name() override;

	private:
		// Fields  
		using game_entry_point = void(*)();
		SDL_SharedObject* game_module_ = nullptr;
	};
}
