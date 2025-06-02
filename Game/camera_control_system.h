#pragma once
#include "../Engine/ECS/Systems/entity_system.h"
#include "../Engine/Events/key_down_event.h"

namespace demo_game::ecs::systems
{
	class camera_control_system final : public sorbengine::ecs::systems::entity_system
	{
	public:
		void init() override;
		void tick(tick_args& args) override;
		std::string get_name() override;
	private:
		void on_key_down(const sorbengine::events::key_down_event& evt);
	};
}
