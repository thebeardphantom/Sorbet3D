#pragma once
#include "../../editor/src/editor_window.h"
#include "../../engine/src/ECS/Components/camera.h"

class camera_editor final : public editor_window
{
public:
	void draw_imgui() override;

private:
	static void draw_camera_gui(sorbengine::ecs::components::camera& cam);
	static void draw_transform_gui(sorbengine::ecs::components::transform& tform);
};
