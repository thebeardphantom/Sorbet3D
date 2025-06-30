#pragma once
#include <glm/ext/vector_float3.hpp>

namespace sorbengine::ecs
{
	struct particular_velocity_system final : public entity_system
	{
	public:
		void tick(tick_args& args) override;
		
	};
}