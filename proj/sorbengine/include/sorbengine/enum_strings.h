#pragma once
#include <string>
#include "engine_api.h"
#include "glad/glad.h"
#include "SDL3/SDL_init.h"

namespace sorbengine
{
	class enum_strings
	{
	public:
		ENGINE_API static std::string to_string(SDL_AppResult value);
		ENGINE_API static std::string to_string(GLenum value);
	};
}
