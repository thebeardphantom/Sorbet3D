#include "sorbengine/modules/input_module.h"
#include <SDL3/SDL_mouse.h>
#include "sorbengine/engine.h"
#include "sorbengine/events/engine_events.h"
#include "sorbengine/modules/render_module.h"

namespace sorbengine::modules
{
	SDL_AppResult input_module::init()
	{
		engine::get_dispatcher().sink<events::void_event>(update).connect<&input_module::on_update>(this);
		return SDL_APP_CONTINUE;
	}

	std::string input_module::get_name()
	{
		return "input_module";
	}

	bool input_module::get_fps_mode() const
	{
		auto& window = engine::get_module<render_module>().get_window();
		return SDL_GetWindowRelativeMouseMode(&window);
	}

	void input_module::set_fps_mode(const bool is_fps_mode)
	{
		auto& window = engine::get_module<render_module>().get_window();
		SDL_SetWindowRelativeMouseMode(&window, is_fps_mode);
	}

	glm::vec2 input_module::get_mouse_velocity() const
	{
		return mouse_velocity_;
	}

	SDL_Keymod input_module::get_keymod() const
	{
		return SDL_GetModState();
	}

	bool input_module::get_key_down(const SDL_Scancode key) const
	{
		return key_states_[key];
	}

	engine_module::event_receive_result input_module::receive_event(const SDL_Event& event)
	{
		auto& dispatcher = engine::get_dispatcher();
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			dispatcher.enqueue<events::key_down_event>(
				events::key_down_event
				{
					.key = event.key.key,
					.mod = event.key.mod
				});
		}
		else if (event.type == SDL_EVENT_KEY_UP)
		{
			dispatcher.enqueue<events::key_up_event>(
				events::key_up_event
				{
					.key = event.key.key,
					.mod = event.key.mod
				});
		}
		else if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			auto& mouse = event.motion;
			mouse_velocity_accumulator_ += glm::vec2(mouse.xrel, mouse.yrel);
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			dispatcher.enqueue<events::mouse_button_down_event>(
				events::mouse_button_down_event
				{
					.button = event.button.button,
				});
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			dispatcher.enqueue<events::mouse_button_up_event>(
				events::mouse_button_up_event
				{
					.button = event.button.button,
				});
		}
		return {false, SDL_APP_CONTINUE};
	}

	void input_module::on_update()
	{
		update_key_states();

		mouse_velocity_ = mouse_velocity_accumulator_;
		mouse_velocity_accumulator_ = glm::vec2();
		SDL_LogTrace(SDL_LOG_CATEGORY_INPUT, "Mouse velocity: (%f, %f)", mouse_velocity_.x, mouse_velocity_.y);
	}

	void input_module::update_key_states()
	{
		int key_states_length = 0;
		const auto immutable_key_states = SDL_GetKeyboardState(&key_states_length);
		if (key_states_ == nullptr)
		{
			key_states_length_ = key_states_length;
			key_states_ = new bool[key_states_length];
		}

		if (key_states_length_ < key_states_length)
		{
			delete[] key_states_;
			key_states_length_ = key_states_length;
			key_states_ = new bool[key_states_length];
		}
		std::memcpy(key_states_, immutable_key_states, key_states_length_);
	}
}
