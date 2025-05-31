#pragma once
#include "../Engine/ECS/Systems/entity_system.h"

namespace demo_game::ecs::systems
{
	class spin_system final : public sorbet::ecs::systems::entity_system
	{
	public:
		void init() override;
		void tick(tick_args& args) override;
		std::string get_name() override;
	};
}
