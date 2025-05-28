#include "../pch.h"
#include "time_module.h"

namespace modules
{
	template <typename T>
	static constexpr auto ns_to_seconds(T ns)
	{
		return ns / 1000000000.0;
	}

	SDL_AppResult time_module::init()
	{
		return SDL_APP_CONTINUE;
	}

	void time_module::cleanup() {}

	void time_module::shutdown() {}

	std::string time_module::get_name()
	{
		return "time_module";
	}

	bool time_module::update()
	{
		const uint64_t ticks_ns = SDL_GetTicksNS();
		time_ = ns_to_seconds(ticks_ns);
		if (last_tick_time_ == 0.0)
		{
			// Avoids a much larger initial deltaTime value
			last_tick_time_ = time_;
		}
		delta_time_ = time_ - last_tick_time_;

		constexpr double_t tick_cooldown = 1.0 / 60.0;
		if (delta_time_ < tick_cooldown)
		{
			return false;
		}

		last_tick_time_ = time_;
		tick_count_++;
		return true;
	}

	ENGINE_API uint64_t time_module::get_tick_count() const
	{
		return tick_count_;
	}

	ENGINE_API double_t time_module::get_delta_time() const
	{
		return delta_time_;
	}

	ENGINE_API double_t time_module::get_time() const
	{
		return time_;
	}
}
