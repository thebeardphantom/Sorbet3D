#include "editor_camera_system.h"
#include <sorbengine/engine.h>
#include <sorbengine/ecs/components/camera.h>
#include <sorbengine/ecs/components/transform.h>
#include <sorbengine/events/engine_events.h>
#include <sorbengine/modules/ecs_module.h>
#include <sorbengine/modules/input_module.h>
#include "sorbeditor/editor_camera.h"

namespace sorbeditor
{
	void editor_camera_system::init()
	{
		sorbengine::engine::get_dispatcher()
			.sink<sorbengine::events::mouse_button_down_event>()
			.connect<&editor_camera_system::on_mouse_button_down>(this);
		sorbengine::engine::get_dispatcher()
			.sink<sorbengine::events::mouse_button_up_event>()
			.connect<&editor_camera_system::on_mouse_button_up>(this);
	}

	void editor_camera_system::tick(tick_args& args)
	{
		auto& input_module = sorbengine::engine::get_module<sorbengine::modules::input_module>();

		const auto view = args.registry.view<
			sorbengine::ecs::camera,
			sorbengine::ecs::transform,
			editor_camera>();

		const auto mouse_velocity = input_module.get_mouse_velocity() * 0.1f;

		const float speed = (input_module.get_keymod() & SDL_KMOD_SHIFT) == 0 ? 1.0f : 3.0f;
		auto local_movement = glm::vec3();
		if (input_module.get_key_down(SDL_SCANCODE_W))
		{
			local_movement.z += speed * static_cast<float>(args.delta_time);
		}
		if (input_module.get_key_down(SDL_SCANCODE_S))
		{
			local_movement.z -= speed * static_cast<float>(args.delta_time);
		}
		if (input_module.get_key_down(SDL_SCANCODE_D))
		{
			local_movement.x += speed * static_cast<float>(args.delta_time);
		}
		if (input_module.get_key_down(SDL_SCANCODE_A))
		{
			local_movement.x -= speed * static_cast<float>(args.delta_time);
		}
		if (input_module.get_key_down(SDL_SCANCODE_E) || input_module.get_key_down(SDL_SCANCODE_SPACE))
		{
			local_movement.y += speed * static_cast<float>(args.delta_time);
		}
		if (input_module.get_key_down(SDL_SCANCODE_Q) || (input_module.get_keymod() & SDL_KMOD_CTRL) != 0)
		{
			local_movement.y -= speed * static_cast<float>(args.delta_time);
		}

		has_active_editor_camera_ = false;
		for (const auto entity : view)
		{
			auto [cam, tform] = view.get<
				sorbengine::ecs::camera,
				sorbengine::ecs::transform,
				editor_camera>(entity);
			if (cam.is_active)
			{
				has_active_editor_camera_ = true;
			}
			if (input_module.get_fps_mode())
			{
				cam.yaw += mouse_velocity.x;
				cam.pitch += mouse_velocity.y;

				const auto global_movement = tform.get_local_rotation() * local_movement;
				tform.local_position += global_movement;
			}
		}

		if (!has_active_editor_camera_)
		{
			input_module.set_fps_mode(false);
		}
	}

	void editor_camera_system::on_mouse_button_down(const sorbengine::events::mouse_button_down_event& evt)
	{
		if (has_active_editor_camera_ && evt.button == 3)
		{
			auto& input_module = sorbengine::engine::get_module<sorbengine::modules::input_module>();
			input_module.set_fps_mode(true);
		}
	}

	void editor_camera_system::on_mouse_button_up(const sorbengine::events::mouse_button_up_event& evt)
	{
		if (evt.button == 3)
		{
			auto& input_module = sorbengine::engine::get_module<sorbengine::modules::input_module>();
			input_module.set_fps_mode(false);
		}
	}
}
