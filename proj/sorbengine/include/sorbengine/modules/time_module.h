#pragma once
#include <SDL3/SDL_init.h>
#include "engine_module.h"

namespace sorbengine::modules
{
	class time_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;
		

		ENGINE_API uint64_t get_update_count() const;
		ENGINE_API double_t get_delta_time() const;
		ENGINE_API double_t get_time() const;

	private:
		uint64_t update_count_ = 0;
		double_t time_last_update_ = 0;
		double_t time_ = 0;
		double_t delta_time_ = 0;

		void on_update();
	};
}
