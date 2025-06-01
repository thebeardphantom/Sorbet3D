#pragma once
#include <glm/vec2.hpp>
#include "engine_module.h"
#include "../Events/engine_events.h"

namespace sorbengine::modules
{
	class input_module : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void collaborate() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;

		[[nodiscard]] glm::vec2 get_mouse_velocity() const;

	private:
		void on_receive_event(const events::receive_event_event& event);
		glm::vec2 mouse_velocity_ = {};
	};
}
