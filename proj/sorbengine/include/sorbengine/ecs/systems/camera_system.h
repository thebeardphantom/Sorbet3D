#pragma once
#include "sorbengine/ecs/systems/entity_system.h"

namespace sorbengine::ecs
{
	class camera_system final : public entity_system
	{
	public:
		void tick(tick_args& args) override;
		
	};
}
