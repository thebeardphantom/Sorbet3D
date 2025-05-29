#pragma once
#include <SDL3/SDL_log.h>

class logging
{
public:
	static void init();

private:
	static void log(void* userdata, int category, SDL_LogPriority priority, const char* message);
	static std::string format_log(const char* message, SDL_LogPriority priority);
	static SDL_LogOutputFunction default_function_;
};
