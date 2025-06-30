#pragma once
#include "SDL3/SDL_loadso.h"
#include "sorbengine/modules/engine_module.h"

namespace sorbengine::modules
{
	class editor_layer_module final : public engine_module
	{
	public:
		// Methods  
		SDL_AppResult init() override;
		void shutdown() override;
		

	private:
		// Fields  
		using game_entry_point = void(*)();
		SDL_SharedObject* editor_so_ = nullptr;
	};
}
