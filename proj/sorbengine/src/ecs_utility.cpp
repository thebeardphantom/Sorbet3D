#include "sorbengine/ecs_utility.h"

namespace sorbengine::utility
{
	Uint32 entity_to_uint(entt::entity entity)
	{
		return static_cast<Uint32>(entity);
	}

	Sint32 entity_to_int(entt::entity entity)
	{
		return static_cast<Sint32>(entity);
	}

	std::string entity_to_string(const entt::entity entity)
	{
		const auto uint = entity_to_uint(entity);
		return std::to_string(uint);
	}
}
