#pragma once
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include "asset_module.h"
#include "engine_module.h"
#include "../Objects/mesh_gpu.h"

namespace modules
{
	class render_module final : public engine_module
	{
	public:
		// Overridden Methods
		SDL_AppResult init() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;

		// Public Methods
		ENGINE_API void submit(const std::shared_ptr<objects::mesh_cpu>& mesh);
		void render();

		// Public Fields
		bool wireframe_mode = false;

	private:
		// Private Methods
		SDL_AppResult init_sdl_window();
		SDL_AppResult init_sdl_open_gl();
		SDL_AppResult init_render_statics();
		void pre_render() const;
		void render_internal();
		void post_render() const;

		// Fields
		SDL_Window* window_ = nullptr;
		SDL_Renderer* renderer_ = nullptr;
		SDL_GLContext gl_context_ = nullptr;
		std::unique_ptr<objects::shader> default_shader_ = nullptr;
		std::vector<std::weak_ptr<objects::mesh_cpu>> render_list_;
	};
}
