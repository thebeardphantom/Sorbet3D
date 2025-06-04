#include "engine_editor.h"
#include <imgui.h>
#include "camera_editor.h"
#include "../Editor/editor.h"


extern "C" __declspec(dllexport) void editor_entry_point(ImGuiContext* context)
{
	ImGui::SetCurrentContext(context);
	engine_editor::init();
}

void engine_editor::init()
{
	sorbeditor::editor::create_window<camera_editor>();
}
