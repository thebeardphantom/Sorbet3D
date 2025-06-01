#include "pch.h"
#include "editor_module.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL_video.h>
#include "../Engine/engine.h"
#include "../Engine/Modules/render_module.h"

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
		dispatcher.sink<sorbengine::events::receive_event_event>().connect<&editor_module::receive_event>(this);
		dispatcher.sink<sorbengine::events::update_event>().connect<&editor_module::update>(this);
		dispatcher.sink<sorbengine::events::render_event>().connect<&editor_module::render>(this);

		return SDL_APP_CONTINUE;
	}

	void editor_module::collaborate()
	{
		const auto& render_module = sorbengine::engine::get_module<sorbengine::modules::render_module>();

		const auto window = render_module.get_window();
		const float display_scale = SDL_GetWindowDisplayScale(window);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplayFramebufferScale = ImVec2(display_scale, display_scale);
		io.FontGlobalScale = display_scale;

		ImGui_ImplSDL3_InitForOpenGL(render_module.get_window(), render_module.get_context());
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

	void editor_module::receive_event(const sorbengine::events::receive_event_event& event)
	{
		ImGui_ImplSDL3_ProcessEvent(&event.event);
	}

	void editor_module::update(const sorbengine::events::update_event& event)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
	}

	void editor_module::render(const sorbengine::events::render_event& event)
	{
		ImGui::Render();
		const auto draw_data = ImGui::GetDrawData();
		ImGui_ImplOpenGL3_RenderDrawData(draw_data);
	}
}
