#pragma once
#include "engine_module.h"

namespace modules
{
	class time_module : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void cleanup() override;
		std::string get_name() override;

		bool update();
		ENGINE_API uint64_t get_tick_count() const;
		ENGINE_API double_t get_delta_time() const;
		ENGINE_API double_t get_time() const;

	private:
		uint64_t tick_count_ = 0;
		double_t last_tick_time_ = 0;
		double_t time_ = 0;
		double_t delta_time_ = 0;
	};
}
