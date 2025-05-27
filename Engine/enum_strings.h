#pragma once
#include "pch.h"

class enum_strings
{
public:
	ENGINE_API static std::string to_string(SDL_AppResult value);
	ENGINE_API static std::string to_string(GLenum value);
};
