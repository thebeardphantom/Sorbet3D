#pragma once
#include "../../Objects/mesh_cpu.h"

namespace sorbet::ecs::components
{
	struct mesh_renderer
	{
		std::shared_ptr<objects::mesh_cpu> mesh;
	};
}
