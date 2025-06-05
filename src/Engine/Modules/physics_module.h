#pragma once
#include <memory>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <foundation/PxFoundation.h>
#include "engine_module.h"

namespace sorbengine::modules
{
	class physics_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void cleanup() override;
		std::string get_name() override;

	private:
		physx::PxDefaultAllocator allocator_;
		physx::PxDefaultErrorCallback error_callback_;
		physx::PxFoundation* foundation_ = nullptr;
	};
}
