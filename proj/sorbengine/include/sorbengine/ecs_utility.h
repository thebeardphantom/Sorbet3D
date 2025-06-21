#pragma once
#include <string>
#include <entt/entt.hpp>
#include <SDL3/SDL_stdinc.h>
#include "sorbengine/engine_api.h"

namespace sorbengine::utility
{
	ENGINE_API std::string entity_to_string(entt::entity entity);
	ENGINE_API Uint32 entity_to_uint(entt::entity entity);
	ENGINE_API Sint32 entity_to_int(entt::entity entity);
}
