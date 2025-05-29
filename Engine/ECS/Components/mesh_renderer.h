#pragma once
#include "../../Objects/mesh_cpu.h"

namespace ecs::components
{
	struct mesh_renderer
	{
		std::shared_ptr<objects::mesh_cpu> mesh;
	};
}
