#include "pch.h"
#include "logging.h"
#include <SDL3/SDL_log.h>
#include "engine_instance.h"

namespace sorbengine
{
	void (*logging::default_function_)(void*, int, SDL_LogPriority, const char*) = nullptr;

	void logging::init(const SDL_LogPriority global_log_priority)
	{
		default_function_ = SDL_GetDefaultLogOutputFunction();
		SDL_SetLogPriorities(global_log_priority);
		SDL_SetLogOutputFunction(log, nullptr);
	}

	void logging::log(void* userdata, const int category, const SDL_LogPriority priority, const char* message)
	{
		const bool is_error = priority == SDL_LOG_PRIORITY_ERROR
			|| priority == SDL_LOG_PRIORITY_CRITICAL;
		const std::string output_msg = format_log(message, priority);
		default_function_(userdata, category, priority, output_msg.c_str());
	}

	std::string logging::format_log(const char* message, const SDL_LogPriority priority)
	{
		const uint64_t ticks = SDL_GetTicks();
		switch (priority)
		{
			case SDL_LOG_PRIORITY_TRACE:
			{
				return std::format("\033[36m[{}] [TRC] {}\033[0m", ticks, message);
			}
			case SDL_LOG_PRIORITY_VERBOSE:
			{
				return std::format("\033[36m[{}] [VER] {}\033[0m", ticks, message);
			}
			case SDL_LOG_PRIORITY_DEBUG:
			{
				return std::format("\033[32m[{}] [DBG] {}\033[0m", ticks, message);
			}
			case SDL_LOG_PRIORITY_INFO:
			{
				return std::format("[{}] [INF] {}", ticks, message);
			}
			case SDL_LOG_PRIORITY_WARN:
			{
				return std::format("\033[33m[{}] [WRN] {}\033[0m", ticks, message);
			}
			case SDL_LOG_PRIORITY_ERROR:
			{
				return std::format("\033[31m[{}] [ERR] {}\033[0m", ticks, message);
			}
			case SDL_LOG_PRIORITY_CRITICAL:
			{
				return std::format("\033[31m[{}] [CRT] {}\033[0m", ticks, message);
			}
			default:
			{
				throw std::invalid_argument("Unknown log priority");
			}
		}
	}

}