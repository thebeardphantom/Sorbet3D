#include "../pch.h"
#include "log_module.h"
#include <iostream>

namespace modules
{
	SDL_AppResult log_module::init()
	{
		SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
		SDL_SetLogOutputFunction([](void* userdata, int category, const SDL_LogPriority priority, const char* message)
		{
			const bool is_error = priority == SDL_LOG_PRIORITY_ERROR || priority == SDL_LOG_PRIORITY_CRITICAL;
			std::ostream& stream = is_error ? std::cerr : std::cout;
			append_prefix(stream, priority);
			stream << message << "\033[0m" << '\n';
		}, nullptr);
		return SDL_APP_CONTINUE;
	}

	void log_module::cleanup() {}

	std::string log_module::get_name()
	{
		return "log_module";
	}

	void log_module::append_prefix(std::ostream& stream, const SDL_LogPriority priority)
	{
		switch (priority)
		{
			case SDL_LOG_PRIORITY_TRACE:
			{
				stream << "\033[36m[TRC] ";
				break;
			}
			case SDL_LOG_PRIORITY_VERBOSE:
			{
				stream << "\033[36m[VER] ";
				break;
			}
			case SDL_LOG_PRIORITY_DEBUG:
			{
				stream << "[DBG] ";
				break;
			}
			case SDL_LOG_PRIORITY_INFO:
			{
				stream << "[INF] ";
				break;
			}
			case SDL_LOG_PRIORITY_WARN:
			{
				stream << "\033[33m[WRN] ";
				break;
			}
			case SDL_LOG_PRIORITY_ERROR:
			{
				stream << "\033[31m[ERR] ";
				break;
			}
			case SDL_LOG_PRIORITY_CRITICAL:
			{
				stream << "\033[31m[CRT] ";
				break;
			}
		}
	}
}
