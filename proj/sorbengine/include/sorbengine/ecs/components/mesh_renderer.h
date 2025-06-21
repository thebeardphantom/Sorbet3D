#pragma once
#include "sorbengine/objects/mesh_cpu.h"

namespace sorbengine::ecs
{
	struct mesh_renderer
	{
		std::shared_ptr<objects::mesh_cpu> mesh;
	};
}
