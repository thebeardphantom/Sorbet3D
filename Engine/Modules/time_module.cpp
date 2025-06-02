#include "../pch.h"
#include "time_module.h"
#include "../engine.h"


namespace sorbengine::modules
{
	template <typename T>
	static constexpr auto ns_to_seconds(T ns)
	{
		return ns / 1000000000.0;
	}

	SDL_AppResult time_module::init()
	{
		auto& dispatcher = engine::get_dispatcher();
		dispatcher.sink<events::void_event>(update).connect<&time_module::on_update>(this);
		return SDL_APP_CONTINUE;
	}

	void time_module::collaborate() {}

	void time_module::cleanup() {}

	void time_module::shutdown() {}

	std::string time_module::get_name()
	{
		return "time_module";
	}

	void time_module::on_update()
	{
		const uint64_t ticks_ns = SDL_GetTicksNS();
		time_ = ns_to_seconds(ticks_ns);
		if (time_last_update_ == 0.0)
		{
			// Avoids a much larger initial deltaTime value
			time_last_update_ = time_;
		}
		delta_time_ = time_ - time_last_update_;
		time_last_update_ = time_;
		update_count++;
	}

	ENGINE_API uint64_t time_module::get_update_count() const
	{
		return update_count;
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
