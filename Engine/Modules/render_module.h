#pragma once
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include "engine_module.h"
#include "../Objects/mesh_gpu.h"

namespace modules
{
	class render_module : public engine_module
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
		static GLuint create_shader(GLenum shader_type, const GLchar* const* src);

		template <typename... T>
		static GLuint create_shader_program(T... shaders)
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Creating shader program.");
			GLuint shader_program = glCreateProgram();
			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Created shader program %d.", shader_program);
			(glAttachShader(shader_program, shaders), ...);

			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Linking shader program %d.", shader_program);
			glLinkProgram(shader_program);

			int success;
			char info[512];
			glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
			if (success)
			{
				SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Activating shader program %d.", shader_program);
				glUseProgram(shader_program);

				SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Deleting attached shaders.");
				(glDeleteShader(shaders), ...);
			}
			else
			{
				glGetProgramInfoLog(shader_program, 512, nullptr, info);
				SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader program %d link error: %s", shader_program, info);
			}
			return shader_program;
		}

		// Fields
		SDL_Window* window_ = nullptr;
		SDL_Renderer* renderer_ = nullptr;
		SDL_GLContext gl_context_ = nullptr;
		GLuint shader_program_ = 0;
		std::vector<std::weak_ptr<objects::mesh_cpu>> render_list_;
	};
}
