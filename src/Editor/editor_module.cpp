#include "editor_module.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL_loadso.h>
#include <SDL3/SDL_video.h>
#include "../Engine/engine.h"
#include "../Engine/Modules/config_module.h"
#include "../Engine/Modules/render_module.h"

using namespace sorbengine::events;

namespace sorbeditor
{
	constexpr auto entry_point_name = "editor_entry_point";

	SDL_AppResult editor_module::init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		auto& dispatcher = sorbengine::engine::get_dispatcher();
		dispatcher.sink<void_event>(update).connect<&editor_module::on_update>(this);
		dispatcher.sink<void_event>(render).connect<&editor_module::on_render>(this);

		return SDL_APP_CONTINUE;
	}

	void editor_module::collaborate()
	{
		auto& config_module = sorbengine::engine::get_module<sorbengine::modules::config_module>();
		auto& config = config_module.get_config();
		const toml::array* arr = config["editor"]["editor_dlls"].as_array();

		for (size_t i = 0; i < arr->size(); i++)
		{
			const auto element = arr->get(i);
			auto dll_name = element->as_string()->get();
			try_load_shared_object(dll_name);
		}

		const auto& render_module = sorbengine::engine::get_module<sorbengine::modules::render_module>();

		const auto window = &render_module.get_window();
		const float display_scale = SDL_GetWindowDisplayScale(window);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplayFramebufferScale = ImVec2(display_scale, display_scale);
		io.FontGlobalScale = display_scale;

		ImGui_ImplSDL3_InitForOpenGL(window, render_module.get_context());
		ImGui_ImplOpenGL3_Init();
	}

	void editor_module::shutdown()
	{
		windows_.clear();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	std::string editor_module::get_name()
	{
		return "editor_module";
	}

	void editor_module::try_load_shared_object(const std::string& name)
	{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Loading %s.", name.c_str());
		const auto result = SDL_LoadObject(name.c_str());
		if (result == nullptr)
		{
			const char* error = SDL_GetError();
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to load %s: %s.", name.c_str(), error);
			return;
		}

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Loading %s().", entry_point_name);
		using setup_im_gui_func = void (*)(ImGuiContext*);
		const auto editor_entry_point = reinterpret_cast<setup_im_gui_func>(
			SDL_LoadFunction(result, entry_point_name));
		if (editor_entry_point == nullptr)
		{
			SDL_UnloadObject(result);
			const char* error = SDL_GetError();
			SDL_LogCritical(
				SDL_LOG_CATEGORY_APPLICATION,
				"Cannot locate %s() in %s: %s.",
				entry_point_name,
				name.c_str(),
				error);
			return;
		}

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Invoking %s.", entry_point_name);
		editor_entry_point(ImGui::GetCurrentContext());

		shared_objects_.push_back(result);
	}

	sorbengine::modules::engine_module::event_receive_result editor_module::receive_event(const SDL_Event& event)
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
		const ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse
			&& (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN
				|| event.type == SDL_EVENT_MOUSE_BUTTON_UP
				|| event.type == SDL_EVENT_MOUSE_WHEEL))
		{
			return {.is_event_used = true, .app_result = SDL_APP_CONTINUE};
		}
		if ((io.WantCaptureKeyboard || io.WantTextInput)
			&& (event.type == SDL_EVENT_KEY_UP || event.type == SDL_EVENT_KEY_DOWN))
		{
			return {.is_event_used = true, .app_result = SDL_APP_CONTINUE};
		}
		return {.is_event_used = false, .app_result = SDL_APP_CONTINUE};
	}

	void editor_module::on_update() const
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		// ImGui::ShowDemoWindow();

		for (const auto& window : windows_)
		{
			window->draw_imgui();
		}
	}

	void editor_module::on_render()
	{
		ImGui::Render();
		const auto draw_data = ImGui::GetDrawData();
		ImGui_ImplOpenGL3_RenderDrawData(draw_data);
	}
}
