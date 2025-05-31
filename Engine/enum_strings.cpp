#include "pch.h"
#include "enum_strings.h"

namespace sorbet
{
	ENGINE_API std::string enum_strings::to_string(const SDL_AppResult value)
	{
		switch (value)
		{
			case SDL_APP_SUCCESS:
				return "SDL_APP_SUCCESS";
			case SDL_APP_CONTINUE:
				return "SDL_APP_CONTINUE";
			case SDL_APP_FAILURE:
				return "SDL_APP_FAILURE";
			default:
				throw std::exception();
		}
	}

	ENGINE_API std::string enum_strings::to_string(GLenum value)
	{
		switch (value)
		{
			case GL_VERTEX_SHADER:
				return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER:
				return "GL_FRAGMENT_SHADER";
			default:
				return std::format("GLenum: %d", value);
		}
	}
}
