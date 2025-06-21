#pragma once
#include <sorbengine/ecs/systems/entity_system.h>

namespace demo_game::ecs
{
	class spin_system final : public sorbengine::ecs::entity_system
	{
	public:
		void tick(tick_args& args) override;
		std::string get_name() override;
	};
}
