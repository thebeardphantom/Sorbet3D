#pragma once
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include "asset_module.h"
#include "engine_module.h"
#include "../engine_api.h"
#include "../event.h"
#include "../render_command.h"

namespace modules
{
	class render_module final : public engine_module
	{
	public:
		static Uint32 log_framerate(void* userdata, SDL_TimerID timer_id, Uint32 interval);

		// Overridden Methods
		SDL_AppResult init() override;
		void collaborate() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;

		// Public Methods
		void submit(const render_command& cmd);
		void render();
		ENGINE_API SDL_Window* get_window() const;
		ENGINE_API SDL_GLContext get_context() const;
		ENGINE_API event<>& get_render_event();

		// Public Fields
		bool wireframe_mode = false;
		bool normals_mode = false;

	private:
		// Private Methods
		SDL_AppResult init_sdl_window();
		SDL_AppResult init_sdl_open_gl();
		SDL_AppResult init_render_statics();
		void pre_render() const;
		void render_internal();
		void post_render() const;

		// Fields
		static uint64_t render_calls_;
		event<> render_event_;
		std::vector<render_command> render_list_;
		SDL_Window* window_ = nullptr;
		SDL_GLContext gl_context_ = nullptr;
		std::shared_ptr<objects::shader> default_shader_ = nullptr;
		std::shared_ptr<objects::shader> normals_shader_ = nullptr;
	};
}
