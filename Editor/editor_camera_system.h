#pragma once
#include "../Engine/ECS/Systems/entity_system.h"
#include "../Engine/Events/engine_events.h"

namespace sorbeditor
{
	class editor_camera_system final : public sorbengine::ecs::systems::entity_system
	{
	public:
		void init() override;
		void tick(tick_args& args) override;
		std::string get_name() override;

	private:
		void on_mouse_button_down(const sorbengine::events::mouse_button_down_event& evt);
		void on_mouse_button_up(const sorbengine::events::mouse_button_up_event& evt);
	};
}
