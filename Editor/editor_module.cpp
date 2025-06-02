#include "pch.h"
#include "editor_module.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL_video.h>
#include "../Engine/engine.h"
#include "../Engine/Modules/render_module.h"

using namespace sorbengine::events;

namespace sorbeditor
{
	SDL_AppResult editor_module::init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		auto& dispatcher = sorbengine::engine::get_dispatcher();
		dispatcher.sink<receive_sdlevent_event>().connect<&editor_module::on_receive_event>(this);
		dispatcher.sink<void_event>(update).connect<&editor_module::on_update>(this);
		dispatcher.sink<void_event>(render).connect<&editor_module::on_render>(this);

		return SDL_APP_CONTINUE;
	}

	void editor_module::collaborate()
	{
		const auto& render_module = sorbengine::engine::get_module<sorbengine::modules::render_module>();

		const auto window = &render_module.get_window();
		const float display_scale = SDL_GetWindowDisplayScale(window);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplayFramebufferScale = ImVec2(display_scale, display_scale);
		io.FontGlobalScale = display_scale;

		ImGui_ImplSDL3_InitForOpenGL(window, render_module.get_context());
		ImGui_ImplOpenGL3_Init();
	}

	void editor_module::cleanup() {}

	void editor_module::shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	std::string editor_module::get_name()
	{
		return "editor_module";
	}

	void editor_module::on_receive_event(const receive_sdlevent_event& event)
	{
		ImGui_ImplSDL3_ProcessEvent(&event.event);
	}

	void editor_module::on_update()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
	}

	void editor_module::on_render()
	{
		ImGui::Render();
		const auto draw_data = ImGui::GetDrawData();
		ImGui_ImplOpenGL3_RenderDrawData(draw_data);
	}
}
