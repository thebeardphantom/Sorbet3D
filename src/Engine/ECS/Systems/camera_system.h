#pragma once
#include "entity_system.h"

namespace sorbengine::ecs::systems
{
	class camera_system final : public entity_system
	{
	public:
		void tick(tick_args& args) override;
		std::string get_name() override;
	};
}
