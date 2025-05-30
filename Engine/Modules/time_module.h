#pragma once
#include "engine_module.h"

namespace modules
{
	class time_module : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void collaborate() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;

		void update();
		ENGINE_API uint64_t get_update_count() const;
		ENGINE_API double_t get_delta_time() const;
		ENGINE_API double_t get_time() const;

	private:
		uint64_t update_count = 0;
		double_t time_last_update_ = 0;
		double_t time_ = 0;
		double_t delta_time_ = 0;
	};
}
