#pragma once
#include "../../engine/src/ECS/Systems/entity_system.h"

namespace demo_game::ecs::systems
{
	class spin_system final : public sorbengine::ecs::systems::entity_system
	{
	public:
		void tick(tick_args& args) override;
		std::string get_name() override;
	};
}
