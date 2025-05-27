#include "pch.h"
#include "enum_strings.h"

ENGINE_API std::string EnumStrings::ToString(SDL_AppResult result)
{
    switch(result)
    {
        case SDL_AppResult::SDL_APP_SUCCESS: return "SDL_APP_SUCCESS";
        case SDL_AppResult::SDL_APP_CONTINUE: return "SDL_APP_CONTINUE";
        case SDL_AppResult::SDL_APP_FAILURE: return "SDL_APP_FAILURE";
        default: throw std::exception();
	}
}

ENGINE_API std::string EnumStrings::ToString(GLenum value)
{
    switch (value)
    {
	    case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
	    case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        default: return std::format("GLenum: %d", value);
    }
}
