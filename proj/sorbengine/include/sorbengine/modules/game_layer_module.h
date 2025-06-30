#pragma once
#include <SDL3/SDL_loadso.h>
#include "sorbengine/reflectable.h"
#include "sorbengine/modules/engine_module.h"

namespace sorbengine::modules
{
	class game_layer_module final : public engine_module
	{
	public:
		// Methods  
		SDL_AppResult init() override;
		void shutdown() override;
		

	private:
		// Fields  
		using game_entry_point = void(*)();
		SDL_SharedObject* game_so_ = nullptr;
	};
}
