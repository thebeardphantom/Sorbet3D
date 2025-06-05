#include "physics_module.h"
#include <foundation/PxPhysicsVersion.h>

namespace sorbengine::modules
{
	SDL_AppResult physics_module::init()
	{
		foundation_ = PxCreateFoundation(
			PX_PHYSICS_VERSION,
			allocator_,
			error_callback_);
		return engine_module::init();
	}

	void physics_module::cleanup()
	{
		engine_module::cleanup();
	}

	std::string physics_module::get_name()
	{
		return "physics_module";
	}
}
