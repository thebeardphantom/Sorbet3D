#include "pch.h"
#include "camera_editor.h"
#include <format>
#include <imgui.h>

#include "../Editor/editor_camera.h"
#include "../Engine/engine.h"
#include "../Engine/engine_instance.h"
#include "../Engine/ECS/Components/camera.h"
#include "../Engine/Modules/ecs_module.h"

using namespace sorbengine::ecs::components;

void camera_editor::draw_imgui()
{
	const auto& ecs_module = sorbengine::engine::get_module<sorbengine::modules::ecs_module>();
	ImGui::Begin("Cameras");
	auto& registry = ecs_module.get_registry();
	const auto view = registry.view<camera,
		transform>(entt::exclude<sorbeditor::editor_camera>);
	for (const auto entity : view)
	{
		const auto id = static_cast<std::uint32_t>(entity);
		ImGui::PushID(id);
		auto [cam, tform] = view.get<camera, transform>(entity);

		ImGui::SeparatorText(std::format("Entity {}", std::to_string(id)).c_str());
		draw_camera_gui(cam);
		draw_transform_gui(tform);
		ImGui::PopID();
	}

	const auto editor_cam_view = registry.view<camera, transform, sorbeditor::editor_camera>();
	for (const auto entity : editor_cam_view)
	{
		const auto id = static_cast<std::uint32_t>(entity);
		ImGui::PushID(id);
		auto [cam, tform] = view.get<camera, transform>(entity);

		ImGui::SeparatorText(std::format("Entity {}", std::to_string(id)).c_str());
		ImGui::Text("Editor Camera");
		draw_camera_gui(cam);
		ImGui::BeginDisabled();
		draw_transform_gui(tform);
		ImGui::EndDisabled();
		ImGui::PopID();
	}
	ImGui::End();
}


void camera_editor::draw_camera_gui(camera& cam)
{
	ImGui::Checkbox("Enabled", &cam.is_enabled);
	ImGui::BeginDisabled();
	ImGui::Checkbox("Active", &cam.is_active);
	ImGui::EndDisabled();
	ImGui::SliderInt("Priority", &cam.priority, -100, 100);
	ImGui::SliderFloat("FOV", &cam.fov, 20.0f, 90.0f);
	ImGui::DragFloat("Yaw", &cam.yaw, 0.1f);
	ImGui::DragFloat("Pitch", &cam.pitch, 0.1f);
}

void camera_editor::draw_transform_gui(transform& tform)
{
	float v[3] = {tform.local_position.x, tform.local_position.y, tform.local_position.z};
	ImGui::DragFloat3("Position", v, 0.1f);
	tform.local_position.x = v[0];
	tform.local_position.y = v[1];
	tform.local_position.z = v[2];

	ImGui::BeginDisabled();
	const auto euler = tform.get_euler_angles();
	v[0] = euler.x;
	v[1] = euler.y;
	v[2] = euler.z;
	ImGui::DragFloat3("Euler", v, 0.1f);
	ImGui::EndDisabled();
}
