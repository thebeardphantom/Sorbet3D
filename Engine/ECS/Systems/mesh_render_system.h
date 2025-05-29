#pragma once
#include "entity_system.h"
#include "../../engine_api.h"

namespace ecs::systems
{
	class mesh_render_system final : public entity_system
	{
	public:
		ENGINE_API void init() override;
		ENGINE_API void tick(tick_args& args) override;
		ENGINE_API std::string get_name() override;
	};
}
