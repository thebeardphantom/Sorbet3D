#include "game_editor.h"
#include <imgui.h>

extern "C" __declspec(dllexport) void editor_entry_point(ImGuiContext* context)
{
	ImGui::SetCurrentContext(context);
	game_editor::init();
}

void game_editor::init() {}
