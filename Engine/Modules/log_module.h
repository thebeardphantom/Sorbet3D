#pragma once
#include <string>
#include "engine_module.h"

namespace modules
{
	class log_module : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;
		static void append_prefix(std::ostream& stream, SDL_LogPriority priority);
	};
}
