#include "../pch.h"
#include "log_module.h"

namespace modules
{
	SDL_AppResult log_module::init()
	{
		SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_TRACE, "[TRC] ");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_VERBOSE, "[VBO] ");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_DEBUG, "[DBG] ");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INFO, "[INF] ");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_WARN, "[WRN] ");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_ERROR, "[ERR] ");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_CRITICAL, "[CRT] ");
		return SDL_APP_CONTINUE;
	}

	void log_module::cleanup() {}

	std::string log_module::get_name()
	{
		return "log_module";
	}
}
