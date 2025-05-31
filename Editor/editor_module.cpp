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
		return SDL_APP_CONTINUE;
	}

	void editor_module::collaborate()
	{
		auto& render_module = sorbengine::engine::get_module<sorbengine::modules::render_module>();

		ImGuiIO& io = ImGui::GetIO();
		const auto window = render_module.get_window();
		const float display_scale = SDL_GetWindowDisplayScale(window);
		io.DisplayFramebufferScale = ImVec2(display_scale, display_scale);
		io.FontGlobalScale = display_scale;

		ImGui_ImplSDL3_InitForOpenGL(render_module.get_window(), render_module.get_context());
		ImGui_ImplOpenGL3_Init();

		sorbengine::engine::get_sdl_event_event().subscribe([this](const SDL_Event& evt)
		{
			ImGui_ImplSDL3_ProcessEvent(&evt);
		});

		sorbengine::engine::get_update_event().subscribe([this]
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow();
		});


		render_module.get_render_event().subscribe([this]
		{
			ImGui::Render();
			const auto draw_data = ImGui::GetDrawData();
			ImGui_ImplOpenGL3_RenderDrawData(draw_data);
		});
	}

	void editor_module::cleanup()
	{
	}

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

}