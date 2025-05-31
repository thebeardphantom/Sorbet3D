#include "../pch.h"
#include "render_module.h"
#include <glm/gtc/type_ptr.hpp>
#include "asset_module.h"
#include "../engine.h"
#include "../Objects/mesh_cpu.h"
#include "../Objects/mesh_gpu.h"

namespace sorbet::modules
{
	uint64_t render_module::render_calls_;

	Uint32 render_module::log_framerate(void* userdata, SDL_TimerID timer_id, Uint32 interval)
	{
		SDL_Log("FPS: %llu", render_calls_);
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
		return result;
	}

	void render_module::collaborate() {}

	void render_module::cleanup()
	{
		render_event_.clear();
		SDL_GL_DestroyContext(gl_context_);
		SDL_DestroyWindow(window_);
	}

	void render_module::shutdown() {}

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

	SDL_Window* render_module::get_window() const
	{
		return window_;
	}

	SDL_GLContext render_module::get_context() const
	{
		return gl_context_;
	}

	std::string render_module::get_name()
	{
		return "render_module";
	}

	event<>& render_module::get_render_event()
	{
		return render_event_;
	}

	void render_module::set_view(const glm::mat4& view)
	{
		view_ = view;
	}

	void render_module::set_projection(const glm::mat4& projection)
	{
		projection_ = projection;
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
		window_ = SDL_CreateWindow("", 800, 600,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
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

		SDL_GL_SetSwapInterval(-1);

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

		render_event_();
	}

	void render_module::post_render() const
	{
		SDL_GL_SwapWindow(window_);
	}
}
