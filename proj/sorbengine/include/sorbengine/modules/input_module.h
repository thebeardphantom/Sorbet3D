#pragma once
#include <glm/vec2.hpp>
#include "sorbengine/modules/engine_module.h"
#include "sorbengine/engine_api.h"
#include "sorbengine/events/engine_events.h"

namespace sorbengine::modules
{
	class input_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;
		std::string get_name() override;
		ENGINE_API bool get_fps_mode() const;
		ENGINE_API void set_fps_mode(bool is_fps_mode);
		ENGINE_API [[nodiscard]] glm::vec2 get_mouse_velocity() const;
		ENGINE_API SDL_Keymod get_keymod() const;
		ENGINE_API bool get_key_down(SDL_Scancode key) const;
		event_receive_result receive_event(const SDL_Event& event) override;

	private:
		glm::vec2 mouse_velocity_ = {};
		glm::vec2 mouse_velocity_accumulator_ = {};
		glm::vec2 mouse_position_ = {};
		void update_key_states();
		void on_update();
		bool* key_states_ = nullptr;
		int key_states_length_ = 0;
	};
}
