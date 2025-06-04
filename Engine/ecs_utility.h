#pragma once
#include <string>
#include <SDL3/SDL_stdinc.h>
#include <entt/entt.hpp>
#include "engine_api.h"

namespace sorbengine::utility::ecs
{
	ENGINE_API std::string entity_to_string(entt::entity entity);
	ENGINE_API Uint32 entity_to_uint(entt::entity entity);
	ENGINE_API Sint32 entity_to_int(entt::entity entity);
}
