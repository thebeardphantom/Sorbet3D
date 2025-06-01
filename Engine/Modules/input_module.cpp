#include "../pch.h"
#include "input_module.h"
#include "../engine.h"
#include "../Events/engine_events.h"

namespace sorbengine::modules
{
	SDL_AppResult input_module::init()
	{
		engine::get_dispatcher().sink<events::receive_event_event>().connect<&input_module::on_receive_event>(this);
		return SDL_APP_CONTINUE;
	}

	void input_module::collaborate() {}

	void input_module::cleanup() {}

	void input_module::shutdown() {}

	std::string input_module::get_name()
	{
		return "input_module";
	}

	glm::vec2 input_module::get_mouse_velocity() const
	{
		return mouse_velocity_;
	}

	void input_module::on_receive_event(const events::receive_event_event& event)
	{
		auto& mouse = event.event.motion;
		mouse_velocity_ = glm::vec2(mouse.xrel, mouse.yrel);
		SDL_Log("%f, %f", mouse_velocity_.x, mouse_velocity_.y);
	}
}
