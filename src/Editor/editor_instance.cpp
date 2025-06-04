#include "editor_instance.h"
#include <entt/entity/registry.hpp>
#include "editor_camera.h"
#include "editor_camera_system.h"
#include "editor_module.h"
#include "../Engine/engine.h"
#include "../Engine/ECS/Components/camera.h"
#include "../Engine/ECS/Components/transform.h"
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
		registry.get_or_emplace<editor_camera>(editor_camera_entity);
		registry.emplace_or_replace<sorbengine::ecs::components::camera>(editor_camera_entity);
		auto& transform = registry.emplace_or_replace<sorbengine::ecs::components::transform>(editor_camera_entity);
		transform.local_position = {0.0f, 0.0f, -2.0f};
	}
}
