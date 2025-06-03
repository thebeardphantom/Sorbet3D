#pragma once
#include "../Editor/editor_window.h"
#include "../Engine/ECS/Components/camera.h"

class camera_editor final : public editor_window
{
public:
	void draw_imgui() override;

private:
	static void draw_camera_gui(sorbengine::ecs::components::camera& cam);
	static void draw_transform_gui(sorbengine::ecs::components::transform& tform);
};
