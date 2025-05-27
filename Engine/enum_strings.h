#pragma once
#include "pch.h"

class EnumStrings
{
public:
	ENGINE_API static std::string ToString(SDL_AppResult value);
	ENGINE_API static std::string ToString(GLenum value);
};

