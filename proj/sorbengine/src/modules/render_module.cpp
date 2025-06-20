#include "sorbengine/modules/render_module.h"
#include <glm/gtc/type_ptr.hpp>
#include <SDL3/SDL_hints.h>
#include "sorbengine/engine.h"
#include "sorbengine/modules/asset_module.h"
#include "sorbengine/modules/config_module.h"
#include "sorbengine/objects/mesh_cpu.h"
#include "sorbengine/objects/mesh_gpu.h"

namespace sorbengine::modules
{
	constexpr Sint32 width_default = 640;
	constexpr Sint32 height_default = 480;
	constexpr SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL
		| SDL_WINDOW_RESIZABLE
		| SDL_WINDOW_HIGH_PIXEL_DENSITY;
	uint64_t render_module::render_calls_;

	Uint32 render_module::log_framerate(void* userdata, SDL_TimerID timer_id, Uint32 interval)
	{
		// SDL_Log("FPS: %llu", render_calls_);
		render_calls_ = 0;
		return 1000;
	}

	SDL_AppResult render_module::init()
	{
		SDL_AppResult result = init_sdl_window();
		if (result != SDL_APP_CONTINUE)
		{
			return result;
		}

		result = init_sdl_open_gl();
		if (result != SDL_APP_CONTINUE)
		{
			return result;
		}

		result = init_render_statics();
		SDL_AddTimer(1000, log_framerate, nullptr);

		auto& dispatcher = engine::get_dispatcher();
		dispatcher.sink<events::key_down_event>()
		          .connect<&render_module::on_key_down>(this);
		return result;
	}

	void render_module::collaborate()
	{
		const auto& config_module = engine::get_module<modules::config_module>();
		const auto display_section = config_module["display"];

		// Width/height
		Sint32 display_width = display_section["width"].value_or(0);
		Sint32 display_height = display_section["height"].value_or(0);
		if (display_width == 0 || display_height == 0)
		{
			const auto window_display = SDL_GetDisplayForWindow(window_);
			const auto display_mode = SDL_GetCurrentDisplayMode(window_display);
			display_width = display_mode->w;
			display_height = display_mode->h;
		}
		SDL_SetWindowSize(window_, display_width, display_height);

		const Sint32 vsync_interval = display_section["vsync"].value_or(1);
		SDL_GL_SetSwapInterval(vsync_interval);

		// Windowed
		const bool windowed = display_section["windowed"].value_or(false);
		SDL_SetWindowFullscreen(window_, !windowed);

		if (windowed)
		{
			if (display_section["maximized"].value_or(false))
			{
				SDL_MaximizeWindow(window_);
			}
			else
			{
				SDL_RestoreWindow(window_);
			}

			const bool borderless = display_section["borderless"].value_or(false);
			SDL_SetWindowBordered(window_, !borderless);

			if (display_section["center_window"].value_or(false))
			{
				SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			}
		}
	}

	void render_module::cleanup()
	{
		SDL_GL_DestroyContext(gl_context_);
		SDL_DestroyWindow(window_);
	}

	void render_module::submit(const render_command& cmd)
	{
		render_list_.push_back(cmd);
	}

	void render_module::render()
	{
		pre_render();
		render_internal();
		post_render();
		render_calls_++;
	}

	SDL_Window& render_module::get_window() const
	{
		return *window_;
	}

	SDL_GLContext render_module::get_context() const
	{
		return gl_context_;
	}

	std::string render_module::get_name()
	{
		return "render_module";
	}

	void render_module::set_view(const glm::mat4& view)
	{
		view_ = view;
	}

	void render_module::set_projection(const glm::mat4& projection)
	{
		projection_ = projection;
	}

	glm::vec2 render_module::get_window_size() const
	{
		Sint32 width;
		Sint32 height;
		SDL_GetWindowSize(window_, &width, &height);
		return {static_cast<float>(width), static_cast<float>(height)};
	}

	float_t render_module::get_window_aspect() const
	{
		const auto size = get_window_size();
		return size.x / size.y;
	}

	void render_module::on_key_down(const events::key_down_event& event)
	{
		if (event.key == SDLK_F1)
		{
			wireframe_mode = !wireframe_mode;
		}
		else if (event.key == SDLK_F2)
		{
			normals_mode = !normals_mode;
		}
	}

	SDL_AppResult render_module::init_sdl_window()
	{
		SDL_Log("== init_sdl_window ==");

		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL_INIT_VIDEO: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
		SDL_SetHint(SDL_HINT_VIDEO_FORCE_EGL, "0");
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Creating window.");
		window_ = SDL_CreateWindow("Sorbet 3D", width_default, height_default, window_flags);
		if (window_ == nullptr)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		return SDL_APP_CONTINUE;
	}

	SDL_AppResult render_module::init_sdl_open_gl()
	{
		SDL_Log(" == init_sdl_open_gl == ");
		gl_context_ = SDL_GL_CreateContext(window_);
		if (gl_context_ == nullptr)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create context: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		// Load OpenGL functions using GLAD (or another loader)
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize GLAD: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));
		return SDL_APP_CONTINUE;
	}

	SDL_AppResult render_module::init_render_statics()
	{
		SDL_Log(" == init_render_statics == ");
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		// Create shaders
		auto& asset_module = engine::get_module<modules::asset_module>();
		default_shader_ = asset_module.load_shader("Engine/Shaders/default");
		normals_shader_ = asset_module.load_shader(
			"Engine/Shaders/normals",
			"Engine/Shaders/default");

		return SDL_APP_CONTINUE;
	}

	void render_module::pre_render() const
	{
		int width;
		int height;
		SDL_GetWindowSize(window_, &width, &height);
		glViewport(0, 0, width, height);

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		engine::get_dispatcher().trigger<events::void_event>(void_events::pre_render);
	}

	void render_module::render_internal()
	{
		if (wireframe_mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		objects::shader* current_shader;
		if (normals_mode)
		{
			current_shader = normals_shader_.get();
		}
		else
		{
			current_shader = default_shader_.get();
		}

		current_shader->use();

		for (const render_command& cmd : render_list_)
		{
			if (const std::shared_ptr<objects::mesh_cpu> mesh_ptr = cmd.mesh.lock())
			{
				const glm::mat4 mvp = projection_ * view_ * cmd.model_matrix;
				const int model_loc = glGetUniformLocation(current_shader->get_id(), "mvp");
				glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(mvp));

				mesh_ptr->get_mesh_gpu().render();
			}
		}
		render_list_.clear();

		engine::get_dispatcher().trigger<events::void_event>(void_events::render);
	}

	void render_module::post_render() const
	{
		SDL_GL_SwapWindow(window_);
		engine::get_dispatcher().trigger<events::void_event>(void_events::post_render);
	}
}
