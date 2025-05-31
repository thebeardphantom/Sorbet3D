#pragma once
#include "engine_module.h"

namespace sorbengine::modules
{
	class editor_layer_module final : public engine_module
	{
	public:
		// Methods  
		SDL_AppResult init() override;
		void collaborate() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;

	private:
		// Fields  
		using game_entry_point = void(*)();
		SDL_SharedObject* editor_so_ = nullptr;
	};
}
