#pragma once
#include "sorbengine/engine_api.h"
#include "sorbengine/ecs/systems/entity_system.h"

namespace sorbengine::ecs::systems
{
	class mesh_render_system final : public entity_system
	{
	public:
		ENGINE_API void tick(tick_args& args) override;
		ENGINE_API std::string get_name() override;
	};
}
