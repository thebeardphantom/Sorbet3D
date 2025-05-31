#pragma once
#include "entity_system.h"

namespace sorbet::ecs::systems
{
	class camera_system final : public entity_system
	{
	public:
		void init() override;
		void tick(tick_args& args) override;
		std::string get_name() override;
	};
}
