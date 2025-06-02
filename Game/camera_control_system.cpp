#include "pch.h"
#include "camera_control_system.h"
#include <glm/gtc/quaternion.hpp>
#include "../Engine/engine.h"
#include "../Engine/ECS/Components/camera.h"
#include "../Engine/ECS/Components/transform.h"
#include "../Engine/Modules/input_module.h"

using namespace sorbengine::ecs::components;

namespace demo_game::ecs::systems
{
	void camera_control_system::init()
	{
		sorbengine::engine::get_dispatcher().sink<sorbengine::events::key_down_event>().connect<&
			camera_control_system::on_key_down>(this);
	}

	void camera_control_system::tick(tick_args& args)
	{
		const auto& input_module = sorbengine::engine::get_module<sorbengine::modules::input_module>();
		if (!input_module.get_fps_mode())
		{
			return;
		}

		const auto view = args.registry.view<camera, transform>();
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
		if (input_module.get_key_down(SDL_SCANCODE_Q) || input_module.get_key_down(SDL_SCANCODE_SPACE))
		{
			local_movement.y += speed * static_cast<float>(args.delta_time);
		}
		if (input_module.get_key_down(SDL_SCANCODE_E) || (input_module.get_keymod() & SDL_KMOD_CTRL) != 0)
		{
			local_movement.y -= speed * static_cast<float>(args.delta_time);
		}

		for (const auto entity : view)
		{
			auto [cam, tform] = view.get<camera, transform>(entity);
			cam.pitch += mouse_velocity.y;
			cam.yaw += mouse_velocity.x;

			const auto global_movement = tform.get_local_rotation() * local_movement;
			tform.local_position += global_movement;
		}
	}

	std::string camera_control_system::get_name()
	{
		return "camera_control_system";
	}

	void camera_control_system::on_key_down(const sorbengine::events::key_down_event& evt)
	{
		if (evt.key == SDLK_ESCAPE)
		{
			auto& input_module = sorbengine::engine::get_module<sorbengine::modules::input_module>();
			input_module.set_fps_mode(!input_module.get_fps_mode());
		}
	}
}
