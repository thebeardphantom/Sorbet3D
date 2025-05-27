#pragma once
#include <SDL3/SDL_init.h>

namespace modules
{
	class engine_module
	{
	public:
		virtual ~engine_module() = default;
		virtual SDL_AppResult init() = 0;
		virtual void cleanup() = 0;
		virtual std::string get_name() = 0;
	};
}
