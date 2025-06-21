#pragma once
#include <sorbeditor/editor_window.h>
#include <sorbengine/ecs/components/camera.h>

class camera_editor final : public editor_window
{
public:
	void draw_imgui() override;

private:
	static void draw_camera_gui(sorbengine::ecs::camera& cam);
	static void draw_transform_gui(sorbengine::ecs::transform& tform);
};
