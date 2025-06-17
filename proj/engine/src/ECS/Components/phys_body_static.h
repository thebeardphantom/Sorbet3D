#pragma once
#include <PxRigidStatic.h>
#include <memory>

namespace sorbengine::ecs::components
{
	struct phys_body_static
	{
		std::unique_ptr<physx::PxFoundation> px_body;
	};
}