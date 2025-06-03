#include "pch.h"
#include "editor_instance.h"

#include "editor_camera.h"
#include "editor_camera_system.h"
#include "editor_module.h"
#include "../Engine/engine.h"
#include "../Engine/Modules/ecs_module.h"

namespace sorbeditor
{
	void editor_instance::init()
	{
		sorbengine::engine::create_module<editor_module>();

		auto& ecs_module = sorbengine::engine::get_module<sorbengine::modules::ecs_module>();
		ecs_module.create_system<editor_camera_system>();

		auto& registry = ecs_module.get_registry();
		const auto editor_camera_entity = registry.create();
		registry.emplace_or_replace<editor_camera>(editor_camera_entity);
		registry.emplace_or_replace<sorbengine::ecs::components::camera>(editor_camera_entity);
		registry.emplace_or_replace<sorbengine::ecs::components::transform>(editor_camera_entity);
	}
}
